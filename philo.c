#include "philo.h"

long long	get_time(void)
{
    struct timeval	tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_status(t_philo *philo, char *status)
{
    long long	current_time;

    pthread_mutex_lock(&philo->data->print_mutex);
    if (!philo->data->someone_died)
    {
        current_time = get_time() - philo->data->start_time;
        printf("%lld %d %s\n", current_time, philo->id + 1, status);
    }
    pthread_mutex_unlock(&philo->data->print_mutex);
}

static void cleanup_resources(t_data *data)
{
    int i;

    i = 0;
    if (data->philosophers)
    {
        pthread_mutex_destroy(&data->print_mutex);
        while (i < data->num_philos)
        {
            pthread_mutex_destroy(&data->forks[i].mutex);
            i++;
        }
        free(data->philosophers);
        free(data->forks);
    }
}

int main(int argc, char **argv)
{
    t_data  data;

    if (argc != 5 && argc != 6)
    {
        printf("Usage: ./philo number_of_philosophers time_to_die ");
        printf("time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
        return (1);
    }
    if (!init_data(&data, argc, argv))
    {
        printf("Error initializing data\n");
        return (1);
    }
    if (!create_threads(&data))
    {
        printf("Error creating threads\n");
        cleanup_resources(&data);
        return (1);
    }
    monitor_philosophers(&data);
    join_threads(&data);
    cleanup_resources(&data);
    return (0);
}
