/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samatsum <samatsum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 14:47:58 by jergashe          #+#    #+#             */
/*   Updated: 2025/01/11 20:37:23 by samatsum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

bool	is_philo_full(t_data *data, t_philo *philo)
{
	bool	result;

	result = false;
	if (get_nb_meals_philo_had(philo) >= data->nb_meals)
		result = true;
	return (result);
}

bool	philo_died(t_philo *philo)
{
	bool		result;
	t_data		*data;

	data = philo->data;
	result = false;
	if (get_time() - get_last_eat_time(philo) > get_die_time(data)
		&& get_philo_state(philo) != EATING)
	{
		set_philo_state(philo, DEAD);
		result = true;
	}
	return (result);
}

void	notify_all_philos(t_data *data)
{
	t_philo	*philos;
	int		i;
	int		nb_philos;

	nb_philos = get_nb_philos(data);
	philos = data->philos;
	i = -1;
	while (++i < nb_philos)
		set_philo_state(&philos[i], DEAD);
}

void	*all_full_routine(void *data_p)
{
	t_data	*data;
	int		i;
	int		nb_philos;
	int		full_Flag;

	data = (t_data *)data_p;
	i = -1;
	full_Flag = 1;
	nb_philos = get_nb_philos(data);
    while (get_keep_iter(data))
    {
        usleep(1000);
        while(++i < nb_philos && get_keep_iter(data))
        {
            full_Flag *= is_philo_full(data, &data->philos[i]);
			if (full_Flag == 0)
                break;
        }
        if (full_Flag)
            break;
        else
        {
            full_Flag = 1;
            i = i - 1;
        }
    }
	if (get_keep_iter(data))
	{
		set_keep_iterating(data, false);
		notify_all_philos(data);
	}
	return (NULL);
}
// usleep(200);

//  || (get_philo_state(&philos[i]) == DEAD
void	*all_alive_routine(void *data_p)
{
	int		i;
	int		nb_philos;
	t_data	*data;
	t_philo	*philos;

	data = (t_data *)data_p;
	philos = data->philos;
	nb_philos = get_nb_philos(data);
	i = -1;
	while (++i < nb_philos && get_keep_iter(data))
	{
		if (philo_died(&philos[i]) && get_keep_iter(data))
		{
			print_msg(data, philos[i].id, DIED);
			set_keep_iterating(data, false);
			notify_all_philos(data);
			break ;
		}
		if (i == nb_philos - 1)
			i = -1;
		usleep(1000);
	}
	return (NULL);
}
// usleep(200);
