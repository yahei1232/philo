/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitorphilo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yal-kham <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 14:01:15 by yal-kham          #+#    #+#             */
/*   Updated: 2025/03/01 14:01:17 by yal-kham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int check_death(t_philo *philo) {
    long long current_time;
    int died = 0;

    pthread_mutex_lock(&philo->data->print_mutex);
    current_time = get_time();
    if (!philo->data->someone_died && (current_time - philo->last_meal > philo->data->time_to_die)) {
        printf("%lld %d died\n", current_time - philo->data->start_time, philo->id + 1);
        philo->data->someone_died = 1;
        died = 1;
    }
    pthread_mutex_unlock(&philo->data->print_mutex);
    return died;
}

void monitor_philosophers(t_data *data) {
    while (1) {
        pthread_mutex_lock(&data->print_mutex);
        if (data->someone_died) {
            pthread_mutex_unlock(&data->print_mutex);
            break;
        }
        pthread_mutex_unlock(&data->print_mutex);

        int i;
        for (i = 0; i < data->num_philos; i++) {
            if (check_death(&data->philosophers[i])) {
                return;
            }
        }

        pthread_mutex_lock(&data->print_mutex);
        bool all_ate = (data->must_eat_count != -1);
        if (all_ate) {
            for (i = 0; i < data->num_philos; i++) {
                if (data->philosophers[i].times_eaten < data->must_eat_count) {
                    all_ate = false;
                    break;
                }
            }
        }
        pthread_mutex_unlock(&data->print_mutex);

        if (all_ate) {
            pthread_mutex_lock(&data->print_mutex);
            data->someone_died = 1;
            pthread_mutex_unlock(&data->print_mutex);
            return;
        }

        usleep(100);
    }
}
