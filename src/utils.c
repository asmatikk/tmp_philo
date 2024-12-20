/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afrikach <afrikach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:06:11 by afrikach          #+#    #+#             */
/*   Updated: 2024/12/13 16:05:03 by afrikach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_atoi(const char *str)
{
	int	nb;
	int	i;
	int	sign;

	nb = 0;
	i = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
	{
		sign = sign * (-1);
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	if (nb > INT_MAX)
		return (-1);
	return (nb * sign);
}

long int	timestamp(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	check_philos_death(t_philo	*philo)
{
	long int	current;
	long int	die;
	long int	eat;

	current = timestamp() - philo->table->start_routine;
	pthread_mutex_lock(&philo->table->death);
	if (philo->table->nb_death >= 1)
	{
		pthread_mutex_unlock(&philo->table->death);
		return (1);
	}
	pthread_mutex_lock(&philo->table->last_m);
	die = (philo->last_meal + philo->table->t_to_die / 1000) - philo->table->start_routine;
	pthread_mutex_unlock(&philo->table->last_m);
	eat = (current + philo->table->t_to_eat / 1000);
	printf("die: %ld eat:%ld\n", die, eat);
	if (die < eat)
	{
		if (philo->table->nb_death == 0)
		{
			philo->table->nb_death++;
			pthread_mutex_unlock(&philo->table->death);
			print_routine_else(philo, 'd');
			pthread_mutex_lock(&philo->table->death);
		}
		pthread_mutex_unlock(&philo->table->death);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->death);
	return (0);
}
