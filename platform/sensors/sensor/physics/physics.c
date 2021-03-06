/**
 * \file
 *
 * \brief ASF Sensor API physics utilities
 *
 * This module defines types and operations that support basic physical
 * modelling and equations for sensor data.
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <platform/sensor/sensor/physics/physics.h>

#define degrees(a)  (RAD_TO_DEG * (a))

/*! \brief Calculate direction, inclination, and field strength
 *
 * This routine calculates horizontal direction, vertical inclination and
 * net field magnitude for a "raw" (unscaled) magnetic \c field sample from
 * a 3-axis magnetometer.
 *
 * \param   field       Magnetometer raw vector field sample input
 * \param   theta       Calculated direction angle (degrees) output
 * \param   delta       Calculated inclination angle (degrees) output
 * \param   strength    Calculated raw vector field magnitude output
 *
 * \return  bool        true if the call succeeds, else false is returned.
 */
bool field_direction(vector3_t *field, scalar_t *theta, scalar_t *delta,
		scalar_t *strength)
{
	/* Calculate the geomagnetic field direction vector (unit vector) and
	 * strength (field magnitude).
	 */
	scalar_t const magnitude = vector3_magnitude(field);

	if (0 == magnitude) {
		return false;
	}

	vector3_scale((1 / magnitude), field);

	*strength = magnitude;

	/* Calculate the direction angle (degrees) assuming no mounting tilt.
	 * "Wraparound" negative results to a positive heading.  The angle is
	 * calculated relative to +Y axis, so atan2() arguments are (x,y)
	 * instead of (y,x).  Result is inverted (* -1) so positive values
	 * reflect clockwise rotation.
	 */
	*theta = degrees(-1 * atan2(field->x, field->y));

	if (*theta < 0) {
		*theta += 360;
	}

	/* Calculate the inclination angle degrees (assuming no mounting tilt).
	 * Downward angle is indicated by a positive inclination value.
	 */
	*delta = degrees(-1 * asin(field->z));

	return true;
}
