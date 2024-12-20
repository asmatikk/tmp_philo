/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afrikach <afrikach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 16:04:26 by afrikach          #+#    #+#             */
/*   Updated: 2024/12/13 17:29:21 by afrikach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	lock_pair_forks(t_philo *philo, t_table *table)
{
	pthread_mutex_lock(&table->forks[philo->left_fork]);
	check_philos_death(philo);
	if (table->nb_death >= 1)
	{
		pthread_mutex_unlock(&table->forks[philo->left_fork]);
		return (1);
	}
	print_routine_eat(philo, 'f');
	pthread_mutex_lock(&table->forks[philo->right_fork]);
	pthread_mutex_lock(&philo->table->death);
	if (table->nb_death >= 1)
	{
		pthread_mutex_unlock(&philo->table->death);
		pthread_mutex_unlock(&table->forks[philo->left_fork]);
		pthread_mutex_unlock(&table->forks[philo->right_fork]);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->death);
	return (0);
}

int	lock_impair_forks(t_philo *philo, t_table *table)
{
	pthread_mutex_lock(&table->forks[philo->right_fork]);
	check_philos_death(philo);
	if (table->nb_death >= 1)
	{
		pthread_mutex_unlock(&table->forks[philo->right_fork]);
		return (1);
	}
	print_routine_eat(philo, 'f');
	pthread_mutex_lock(&table->forks[philo->left_fork]);
	pthread_mutex_lock(&philo->table->death);
	if (table->nb_death >= 1)
	{
		pthread_mutex_unlock(&philo->table->death);
		pthread_mutex_unlock(&table->forks[philo->right_fork]);
		pthread_mutex_unlock(&table->forks[philo->left_fork]);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->death);
	return (0);
}

int	check_impair_philos_death(t_philo *philo, t_table *table)
{
	(void)table;
	print_routine_eat(philo, 'f');
	check_philos_death(philo);
	if (table->nb_death >= 1)
	{
		pthread_mutex_unlock(&table->forks[philo->left_fork]);
		pthread_mutex_unlock(&table->forks[philo->right_fork]);
		return (1);
	}
	return (0);
}

int	check_pair_philos_death(t_philo *philo, t_table *table)
{
	print_routine_eat(philo, 'f');
	check_philos_death(philo);
	pthread_mutex_lock(&philo->table->death);
	if (table->nb_death >= 1)
	{
		pthread_mutex_unlock(&philo->table->death);
		pthread_mutex_unlock(&table->forks[philo->right_fork]);
		pthread_mutex_unlock(&table->forks[philo->left_fork]);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->death);
	return (0);
}

void	philo_eat(t_philo *philo, t_table *table)
{
	if (philo->id % 2 != 0)
	{
		if (lock_impair_forks(philo, table) == 1)
			return ;
		if (check_impair_philos_death(philo, table) == 1)
			return ;
		print_routine_eat(philo, 'e');
		philo->nb_meals++;
		pthread_mutex_unlock(&table->forks[philo->left_fork]);
		pthread_mutex_unlock(&table->forks[philo->right_fork]);
	}
	else
	{
		if (lock_pair_forks(philo, table) == 1)
			return ;
		if (check_pair_philos_death(philo, table) == 1)
			return ;
		print_routine_eat(philo, 'e');
		philo->nb_meals++;
		pthread_mutex_unlock(&table->forks[philo->right_fork]);
		pthread_mutex_unlock(&table->forks[philo->left_fork]);
	}
}
