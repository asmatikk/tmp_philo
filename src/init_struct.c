/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afrikach <afrikach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:26:51 by afrikach          #+#    #+#             */
/*   Updated: 2024/12/13 17:07:38 by afrikach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

// initialisation des structures pour les philos et de la table
// initialisation des forks (Mutex) et des philos
void	init_struct_table(t_table *table, int ac, char **av)
{
	table->nb_philo = ft_atoi(av[1]);
	table->t_to_die = ft_atoi(av[2]) * 1000;
	table->t_to_eat = ft_atoi(av[3]) * 1000;
	table->t_to_sleep = ft_atoi(av[4]) * 1000;
	table->times_must_eat = -1;
	if (ac == 6)
		table->times_must_eat = ft_atoi(av[5]);
	table->philo = malloc(sizeof(t_philo) * table->nb_philo);
	table->forks = malloc(sizeof(pthread_mutex_t) * table->nb_philo);
	if (!table->forks || !table->philo)
	{
		free (table->forks);
		free (table->philo);
		return ;
	}
	pthread_mutex_init(&table->death, NULL);
	pthread_mutex_init(&table->print, NULL);
	pthread_mutex_init(&table->last_m, NULL);
	table->nb_death = 0;
	table->start_routine = timestamp();
	init_struct_philo(table);
}

void	init_struct_philo(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		table->philo[i].table = table;
		pthread_mutex_init(&table->forks[i], NULL);
		table->philo[i].id = i + 1;
		table->philo[i].left_fork = i;
		table->philo[i].right_fork = (i + 1) % table->nb_philo;
		table->philo[i].nb_meals = 0;
		i++;
	}
}
