static int check_death(t_philo *philo)
{
    long long current_time;

    current_time = get_time();
    if (current_time - philo->last_meal > philo->data->time_to_die)
    {
        if (!philo->data->someone_died)
        {
            printf("%lld %d died\n",
                current_time - philo->data->start_time, philo->id + 1);
            pthread_mutex_lock(&philo->data->print_mutex);
            philo->data->someone_died = 1;
            pthread_mutex_unlock(&philo->data->print_mutex);
        }
        return (1);
    }
    return (0);
}

static int check_all_ate(t_data *data)
{
    int i;

    i = 0;
    if (data->must_eat_count == -1)
        return (0);
    while (i < data->must_eat_count)
    {
        if (data->philosophers[i].times_eaten < data->must_eat_count)
            return (1);
        i++;
    }
    return (0);
}

void monitor_philosophers(t_data *data)
{
        int i;

        while (1) // Loop indefinitely until death
        {
            i = 0; // Reset index for each full iteration
            while (i < data->num_philos)
            {
                if (check_death(&data->philosophers[i]) || check_all_ate(&data->philosophers[i]))
                return; // Exit immediately if any philo dies
            i++;
        }

        // Optional: Add small sleep to reduce CPU usage
        usleep(1000); // Or use dynamic_sleep(1, data) for precision

        // Remove the "check_all_ate" condition entirely
    }
}
