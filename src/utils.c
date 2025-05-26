/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: idakhlao <idakhlao@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:06:11 by afrikach          #+#    #+#             */
/*   Updated: 2024/12/20 19:45:46 by idakhlao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	ft_atoi(const char *str)
{
	long int	nb;
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
		if (nb > INT_MAX)
			return (-1);
	}
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
    long int	last;

    current = timestamp() - philo->table->start_routine;
    pthread_mutex_lock(&philo->table->death);
    if (philo->table->nb_death >= 1)
    {
        pthread_mutex_unlock(&philo->table->death);
        return (1);
    }
    pthread_mutex_unlock(&philo->table->death);
    pthread_mutex_lock(&philo->table->last_m);
    last = philo->last_meal;
    pthread_mutex_unlock(&philo->table->last_m);
    if (current - last > (philo->table->t_to_die / 1000) || philo->table->t_to_eat > philo->table->t_to_die || philo->table->t_to_sleep > philo->table->t_to_die)
    {
        pthread_mutex_lock(&philo->table->death);
        if (philo->table->nb_death >= 1)
        {
            pthread_mutex_unlock(&philo->table->death);
            return (1);
        }
       	pthread_mutex_unlock(&philo->table->death);
		pthread_mutex_lock(&philo->table->death);
		if (philo->table->nb_death == 0)
		{
			philo->table->nb_death++;
			pthread_mutex_unlock(&philo->table->death);
			print_routine_else(philo, 'd');
			pthread_mutex_lock(&philo->table->death);
		}
		if (((philo->table->nb_philo -1) * (philo->table->t_to_eat + philo->table->t_to_sleep)) > philo->table->t_to_die)
		{
			pthread_mutex_unlock(&philo->table->death);
			return (1);
		}
		pthread_mutex_unlock(&philo->table->death);
		return (0);
    }
    return (0);
}

void    precise_timer(long timing, t_philo *philo)
{
    long    start_time;

    start_time = timestamp();
    while ((timestamp() - start_time) *1000 < timing)
    {
        if (check_philos_death(philo) == 1)
            break ;
        pthread_mutex_lock(&philo->table->death);
        if (philo->table->nb_death >= 1)
        {
            pthread_mutex_unlock(&philo->table->death);
            break ;
        }
        pthread_mutex_unlock(&philo->table->death);
		 if (check_philos_death(philo) == 1)
            break ;
        usleep(500);
    }
}

void	is_thinking(t_philo *philo)
{
	long	current_time;
	long	time_remaining;

	current_time = timestamp() - philo->table->start_routine;
	pthread_mutex_lock(&philo->table->last_m);
	time_remaining = philo->table->t_to_die - (current_time - philo->last_meal);
	pthread_mutex_unlock(&philo->table->last_m);
	if (time_remaining > 0)
		precise_timer((time_remaining / 2) * 1000, philo);
}

void	*monitoring(void *arg)
{
    t_table *table = (t_table *)arg;
    int		i;

    while (1)
    {
        i = 0;
        while (i < table->nb_philo)
        {
			if (check_philos_death(&table->philo[i]))
            {
				pthread_mutex_lock(&table->death);
                table->nb_death++;
                pthread_mutex_unlock(&table->death);
                return (NULL);
            }
            i++;
        }
        usleep(500);
    }
    return (NULL);
}
