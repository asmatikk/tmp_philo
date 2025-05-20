/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afrikach <afrikach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:26:51 by afrikach          #+#    #+#             */
/*   Updated: 2024/12/13 17:30:21 by afrikach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_table	*table;
	int		eat;

	philo = (t_philo *)arg;
	table = philo->table;
	eat = table->times_must_eat;
	if (philo->table->nb_philo == 1)
	{
		if (philo_alone(philo) == 1)
			return (NULL);
	}
	while (1)
	{
		philo_eat(philo, table);
		if (eat != -1 && eat == philo->nb_meals)
			break ;
		if (check_philos_death(philo) == 1)
			break ;
		print_routine_else(philo, 's');
		if (check_philos_death(philo) == 1)
			break ;
		print_routine_else(philo, 't');
		usleep(1000);
	}
	return (NULL);
}

void	destroy_mutex(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
		pthread_join(table->philo[i++].thread, NULL);
	i = 0;
	while (i < table->nb_philo)
		pthread_mutex_destroy(&table->forks[i++]);
	pthread_mutex_destroy(&table->print);
	pthread_mutex_destroy(&table->death);
	pthread_mutex_destroy(&table->last_m);
	free(table->forks);
	free(table->philo);
	free(table);
}
int    main(int ac, char **av)
{
    t_table    *table;
    int        i;

    table = NULL;
    if (check_input(ac, av) == 1)
        return (printf("erreur input\n"), 1);
    table = malloc(sizeof(t_table));
    if (!table)
        return (0);
    init_struct_table(table, ac, av);
    i = 0;
    while (i < table->nb_philo)
    {
	    table->philo[i].last_meal = 0;
        pthread_create(&table->philo[i].thread,
            NULL, &philo_routine, &table->philo[i]);
        usleep(200);
        i++;
    }
    destroy_mutex(table);
    return (0);
}

// int	main(int ac, char **av)
// {
// 	t_table	*table;
// 	int		i;

// 	table = NULL;
// 	if (check_input(ac, av) == 1)
// 		return (printf("erreur input\n"), 1);
// 	table = malloc(sizeof(t_table));
// 	if (!table)
// 		return (0);
// 	init_struct_table(table, ac, av);
// 	i = 0;
// 	while (i < table->nb_philo)
// 	{
// 		table->philo[i].last_meal = timestamp() + table->t_to_die / 1000;
// 		pthread_create(&table->philo[i].thread,
// 			NULL, &philo_routine, &table->philo[i]);
// 		usleep(200);
// 		i++;
// 	}
// 	destroy_mutex(table);
// 	return (0);
// }
