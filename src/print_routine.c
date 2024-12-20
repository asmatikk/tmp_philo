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

int	philo_alone(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->death);
	if (philo->table->nb_death >= 1)
	{
		pthread_mutex_unlock(&philo->table->death);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->death);
	if (philo->table->nb_philo == 1)
	{
		usleep(philo->table->t_to_die);
		print_routine_else(philo, 'd');
		return (1);
	}
	print_routine_else(philo, 't');
	return (0);
}

void	print_routine_eat(t_philo *philo, char c)
{
	long int	time;

	pthread_mutex_lock(&philo->table->death);
	if (philo->table->nb_death >= 1)
	{
		pthread_mutex_unlock(&philo->table->death);
		return ;
	}
	pthread_mutex_unlock(&philo->table->death);
	pthread_mutex_lock(&philo->table->print);
	time = timestamp() - philo->table->start_routine;
	if (c == 'f')
	{
		printf("%s%ld %d has taken a fork%s\n",BWHITE, time, philo->id, RESET);
		pthread_mutex_unlock(&philo->table->print);
	}
	if (c == 'e')
	{
		printf("%s%ld %d is eating%s\n", BRED, time, philo->id, RESET);
		pthread_mutex_unlock(&philo->table->print);
		usleep(philo->table->t_to_eat);
		pthread_mutex_lock(&philo->table->last_m);
		philo->last_meal = timestamp();
		pthread_mutex_unlock(&philo->table->last_m);
	}
}

void	print_routine_else(t_philo *philo, char c)
{
	long int	time;

	pthread_mutex_lock(&philo->table->death);
	if (philo->table->nb_death > 1)
	{
		pthread_mutex_unlock(&philo->table->death);
		return ;
	}
	pthread_mutex_unlock(&philo->table->death);
	pthread_mutex_lock(&philo->table->print);
	time = timestamp() - philo->table->start_routine;
	if (c == 's')
	{
		printf("%s%ld %d is sleeping%s\n", BBLUE, time, philo->id, RESET);
		pthread_mutex_unlock(&philo->table->print);
		usleep(philo->table->t_to_sleep);
	}
	if (c == 't')
	{
		printf("%s%ld %d is thinking%s\n", BGREEN, time, philo->id, RESET);
		pthread_mutex_unlock(&philo->table->print);
	}
	if (c == 'd')
	{
		printf("%s%ld %d died%s\n", BMAG, time, philo->id, RESET);
		pthread_mutex_unlock(&philo->table->print);
	}
}
