#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_fork
{
    pthread_mutex_t     mutex;
    int                 id;
}   t_fork;

typedef struct s_philo
{
    int             id;
    int             times_eaten;
    long long       last_meal;
    t_fork          *left_fork;
    t_fork          *right_fork;
    struct s_data   *data;
    pthread_t       thread;
}   t_philo;

typedef struct s_data
{
    int             num_philos;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             must_eat_count;
    int             someone_died;
    long long       start_time;
    pthread_mutex_t print_mutex;
    t_philo         *philosophers;
    t_fork          *forks;
}   t_data;

// Function prototypes
long long    get_time(void);
void        print_status(t_philo *philo, char *status);
int         init_data(t_data *data, int argc, char **argv);
void        *philosopher_routine(void *arg);
int         create_threads(t_data *data);
void        join_threads(t_data *data);
void        monitor_philosophers(t_data *data);

#endif