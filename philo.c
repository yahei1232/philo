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

    if (data->philosophers)
    {
        i = 0;
        while (i < data->num_philos)
        {
            pthread_mutex_destroy(&data->forks[i].mutex);
            i++;
        }
        pthread_mutex_destroy(&data->print_mutex);
        free(data->philosophers);
        free(data->forks);
    }
}

static int take_forks(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->print_mutex);
    if (philo->data->someone_died)
    {
        pthread_mutex_unlock(&philo->data->print_mutex);
        return (0);
    }
    pthread_mutex_unlock(&philo->data->print_mutex);

    // Take forks in order to prevent deadlock
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&philo->right_fork->mutex);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(&philo->left_fork->mutex);
        print_status(philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(&philo->left_fork->mutex);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(&philo->right_fork->mutex);
        print_status(philo, "has taken a fork");
    }
    return (1);
}

static void release_forks(t_philo *philo)
{
    pthread_mutex_unlock(&philo->left_fork->mutex);
    pthread_mutex_unlock(&philo->right_fork->mutex);
}

static int eat(t_philo *philo)
{
    if (!take_forks(philo))
        return (0);

    // Update last_meal and times_eaten under the same mutex lock
    pthread_mutex_lock(&philo->data->print_mutex);
    philo->last_meal = get_time();
    philo->times_eaten++;
    print_status(philo, "is eating");
    pthread_mutex_unlock(&philo->data->print_mutex);

    usleep(philo->data->time_to_eat * 1000);
    release_forks(philo);
    return (1);
}

void *philosopher_routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    if (philo->data->num_philos == 1)
    {
        print_status(philo, "has taken a fork");
        usleep(philo->data->time_to_die * 1000);
        return (NULL);
    }

    // Stagger even numbered philosophers to prevent deadlock
    if (philo->id % 2 == 0)
        usleep(1000);

    while (1)
    {
        pthread_mutex_lock(&philo->data->print_mutex);
        if (philo->data->someone_died)
        {
            pthread_mutex_unlock(&philo->data->print_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->data->print_mutex);

        if (!eat(philo))
            break;

        print_status(philo, "is sleeping");
        usleep(philo->data->time_to_sleep * 1000);

        print_status(philo, "is thinking");
    }
    return (NULL);
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
