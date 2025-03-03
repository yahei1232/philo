#include "philo.h"

static void take_forks(t_philo *philo)
{
    // Determine which fork has lower ID
    t_fork *first_fork;
    t_fork *second_fork;
    
    if (philo->left_fork->id < philo->right_fork->id) {
        first_fork = philo->left_fork;
        second_fork = philo->right_fork;
    } else {
        first_fork = philo->right_fork;
        second_fork = philo->left_fork;
    }
    
    // Take forks in consistent order
    pthread_mutex_lock(&first_fork->mutex);
    print_status(philo, "has taken a fork");
    pthread_mutex_lock(&second_fork->mutex);
    print_status(philo, "has taken a fork");
}

static void eat(t_philo *philo)
{
    print_status(philo, "is eating");
    
    // Use mutex to protect last_meal update
    pthread_mutex_lock(&philo->data->print_mutex);
    philo->last_meal = get_time();
    philo->times_eaten++;
    pthread_mutex_unlock(&philo->data->print_mutex);
    
    usleep(philo->data->time_to_eat * 1000);
    pthread_mutex_unlock(&philo->right_fork->mutex);
    pthread_mutex_unlock(&philo->left_fork->mutex);
}

static void sleep_and_think(t_philo *philo)
{
    print_status(philo, "is sleeping");
    usleep(philo->data->time_to_sleep * 1000);
    print_status(philo, "is thinking");
}

void *philosopher_routine(void *arg) {
    t_philo *philo = (t_philo *)arg;

    if (philo->id % 2)
        usleep(1000);

    while (1) {
        pthread_mutex_lock(&philo->data->print_mutex);
        if (philo->data->someone_died) {
            pthread_mutex_unlock(&philo->data->print_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->data->print_mutex);

        take_forks(philo);
        eat(philo);
        sleep_and_think(philo);

        pthread_mutex_lock(&philo->data->print_mutex);
        if (philo->data->must_eat_count != -1 && philo->times_eaten >= philo->data->must_eat_count) {
            pthread_mutex_unlock(&philo->data->print_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->data->print_mutex);
    }
    return NULL;
}
