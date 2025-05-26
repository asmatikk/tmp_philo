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
	pthread_mutex_lock(&philo->table->death);
	if (table->nb_death >= 1)
	{
		pthread_mutex_unlock(&table->forks[philo->left_fork]);
		pthread_mutex_unlock(&philo->table->death);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->death);
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
	pthread_mutex_lock(&philo->table->death);
	if (table->nb_death >= 1)
	{
		pthread_mutex_unlock(&table->forks[philo->right_fork]);
		pthread_mutex_unlock(&philo->table->death);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->death);
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
    print_routine_eat(philo, 'f');
    check_philos_death(philo);
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
    int locked = 0;
    int unlocked_by_death = 0;

    if (philo->id % 2 != 0)
    {
        if (lock_impair_forks(philo, table) == 1)
            return ;
        locked = 1;
        unlocked_by_death = check_impair_philos_death(philo, table);
        if (unlocked_by_death == 1)
            return ;
        print_routine_eat(philo, 'e');
        philo->nb_meals++;
    }
    else
    {
        if (lock_pair_forks(philo, table) == 1)
            return ;
        locked = 1;
        unlocked_by_death = check_pair_philos_death(philo, table);
        if (unlocked_by_death == 1)
            return ;
        print_routine_eat(philo, 'e');
        philo->nb_meals++;
    }
    if (locked && !unlocked_by_death)
    {
        pthread_mutex_unlock(&table->forks[philo->left_fork]);
        pthread_mutex_unlock(&table->forks[philo->right_fork]);
    }
}
