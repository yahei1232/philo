/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitorphilo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 19:33:09 by hassende          #+#    #+#             */
/*   Updated: 2024/03/03 22:40:29 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int check_philosopher_death(t_philo *philo)
{
    long long current_time;

    current_time = get_time();
    pthread_mutex_lock(&philo->data->print_mutex);
    if ((current_time - philo->last_meal) > philo->data->time_to_die)
    {
        print_status(philo, "died");
        philo->data->someone_died = 1;
        pthread_mutex_unlock(&philo->data->print_mutex);
        return (1);
    }
    pthread_mutex_unlock(&philo->data->print_mutex);
    return (0);
}

static int check_all_philosophers_ate(t_data *data)
{
    int i;
    int all_ate;

    if (data->must_eat_count < 0)
        return (0);
    
    all_ate = 1;
    i = 0;
    pthread_mutex_lock(&data->print_mutex);
    while (i < data->num_philos)
    {
        if (data->philosophers[i].times_eaten < data->must_eat_count)
        {
            all_ate = 0;
            break;
        }
        i++;
    }
    pthread_mutex_unlock(&data->print_mutex);
    return (all_ate);
}

void monitor_philosophers(t_data *data)
{
    int i;

    while (1)
    {
        i = 0;
        while (i < data->num_philos)
        {
            if (check_philosopher_death(&data->philosophers[i]))
                return;
            i++;
        }

        if (check_all_philosophers_ate(data))
        {
            pthread_mutex_lock(&data->print_mutex);
            printf("All philosophers have eaten enough times!\n");
            data->someone_died = 1;  // Use this flag to stop simulation
            pthread_mutex_unlock(&data->print_mutex);
            return;
        }
        usleep(1000);  // Small sleep to prevent CPU overuse
    }
}
