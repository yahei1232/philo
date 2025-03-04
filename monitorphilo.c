#include "philo.h"

static int check_death(t_philo *philo)
{
    long long current_time;

    pthread_mutex_lock(&philo->data->print_mutex);
    current_time = get_time();
    if (current_time - philo->last_meal > philo->data->time_to_die && !philo->data->someone_died)
    {
        printf("%lld %d died\n",
            current_time - philo->data->start_time, philo->id + 1);
        philo->data->someone_died = 1;
        return 0;
    }
    pthread_mutex_unlock(&philo->data->print_mutex);
    return 1;
}

static int check_all_ate(t_data *data)
{
    int i;

    i = 0;
    if (data->must_eat_count == -1)
        return (0);
    while (i < data->num_philos)
    {
        if (data->philosophers[i].times_eaten < data->must_eat_count)
            return (0);
        i++;
    }
    return (1);
}

void monitor_philosophers(t_data *data)
{
    int i;

    while (!data->someone_died)
    {
    	i = 0;
        while (i < data->num_philos)
        {
            if (check_death(&data->philosophers[i]))
                return;
            i++;
        }
        if (check_all_ate(data))
        {
            pthread_mutex_lock(&data->print_mutex);
            data->someone_died = 1;
            pthread_mutex_unlock(&data->print_mutex);
            return;
        }
        usleep(1000);
    }
}
