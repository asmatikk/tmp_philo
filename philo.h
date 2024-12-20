/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afrikach <afrikach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:27:06 by afrikach          #+#    #+#             */
/*   Updated: 2024/12/13 17:29:41 by afrikach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>

# define BBLACK "\033[1;30m"
# define BRED "\033[1;31m"
# define BGREEN "\033[1;32m"
# define BYELLOW "\033[1;33m"
# define BBLUE "\033[1;34m"
# define BMAG "\033[1;35m"
# define BCYAN "\033[1;36m"
# define BWHITE "\033[1;37m"
# define RESET "\033[0m"

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	int				left_fork;
	int				right_fork;
	long int		start_eating;
	int				nb_meals;
	long int		last_meal;
	struct s_table	*table;
}					t_philo;

typedef struct s_table
{
	int				nb_philo;
	pthread_mutex_t	*forks;
	pthread_mutex_t	death;
	pthread_mutex_t	print;
	pthread_mutex_t last_m;
	int				nb_death;
	int				t_to_die;
	int				t_to_eat;
	int				t_to_sleep;
	int				times_must_eat;
	long int		start_routine;
	t_philo			*philo;
}					t_table;

int			check_input(int ac, char **av);
void		init_struct_table(t_table *table, int ac, char **av);
void		init_struct_philo(t_table *table);
int			ft_atoi(const char *str);
void		*philo_routine(void *arg);
int			check_free_forks(t_philo *philo, int id);
long int	timestamp(void);
void		print_routine_eat(t_philo *philo, char c);
void		print_routine_else(t_philo *philo, char c);
int			check_philos_death(t_philo	*philo);
void		philo_eat(t_philo *philo, t_table *table);
int			philo_alone(t_philo *philo);

#endif
