#include "philo.h"

Data* initialize_data(size_t initial_capacity) {
    Data* data = (Data*)malloc(sizeof(Data));
    if (data == NULL) {
        return NULL;
    }
    
    data->array = (int*)malloc(initial_capacity * sizeof(int));
    if (data->array == NULL) {
        free(data);
        return NULL;
    }
    
    data->size = 0;
    data->capacity = initial_capacity;
    
    return data;
}

void free_data(Data* data) {
    if (data != NULL) {
        free(data->array);
        free(data);
    }
}

static int parse_input(t_data *data, int argc, char **argv)
{
    data->num_philos = atoi(argv[1]);
    data->time_to_die = atoi(argv[2]);
    data->time_to_eat = atoi(argv[3]);
    data->time_to_sleep = atoi(argv[4]);
    if (argc == 6)
        data->must_eat_count = atoi(argv[5]);
    else
        data->must_eat_count =  -1;
    data->someone_died = 0;
    if (data->num_philos <= 0 || data->time_to_die <= 0 ||
        data->time_to_eat <= 0 || data->time_to_sleep <= 0 ||
        (argc == 6 && data->must_eat_count <= 0))
        return (0);
    return (1);
}

static int init_mutexes(t_data *data)
{
    int i;

    i = 0;
    if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
        return (0);
    if (pthread_mutex_init(&data->someone_died, NULL) != 0)
        return (0);

    while (i < data->num_philos)
    {
        if (pthread_mutex_init(&data->forks[i].mutex, NULL) != 0)
            return (0);
        data->forks[i].id = i;
        i++;
    }
    return (1);
}

static int init_philosophers(t_data *data)
{
    int i;
    long long current_time;

    i = 0;
    current_time = get_time();
    while (i < data->num_philos)
    {
        data->philosophers[i].id = i;
        data->philosophers[i].times_eaten = 0;
        data->philosophers[i].last_meal = current_time;
        data->philosophers[i].data = data;
        data->philosophers[i].left_fork = &data->forks[i];
        data->philosophers[i].right_fork = &data->forks[(i + 1) % data->num_philos];
        i++;
    }
    return (1);
}

int init_data(t_data *data, int argc, char **argv)
{
    if (!parse_input(data, argc, argv))
        return (0);

    data->philosophers = malloc(sizeof(t_philo) * data->num_philos);
    if (!data->philosophers)
        return (0);

    data->forks = malloc(sizeof(t_fork) * data->num_philos);
    if (!data->forks)
    {
        free(data->philosophers);
        return (0);
    }

    if (!init_mutexes(data) || !init_philosophers(data))
    {
        free(data->philosophers);
        free(data->forks);
        return (0);
    }

    data->start_time = get_time();
    return (1);
}
