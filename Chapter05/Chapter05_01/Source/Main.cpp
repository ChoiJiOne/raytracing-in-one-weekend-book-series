#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

bool hit_sphere(const point3& center, double radius, const ray& r)
{
	vec3 oc = center - r.origin();
	double a = dot(r.direction(), r.direction());
	double b = -2.0 * dot(r.direction(), oc);
	double c = dot(oc, oc) - radius * radius;
	double discriminant = b * b - 4.0 * a * c;
	return (discriminant >= 0.0);
}

color ray_color(const ray& r)
{
	if (hit_sphere(point3(0.0, 0.0, -1.0), 0.5, r))
	{
		return color(1.0, 0.0, 0.0);
	}

	vec3 unit_direction = unit_vector(r.direction());
	double a = 0.5 * (unit_direction.y() + 1.0f);
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main(void)
{
	double aspect_ratio = 16.0 / 9.0;
	int image_width = 400;
	int image_height = static_cast<int>(static_cast<double>(image_width) / aspect_ratio);
	image_height = std::max(1, image_height);

	double focal_length = 1.0;
	double viewport_height = 2.0f;
	double viewport_width = viewport_height * (static_cast<double>(image_width) / static_cast<double>(image_height));
	point3 camera_center = point3(0.0, 0.0, 0.0);

	vec3 viewport_u = vec3(viewport_width, 0.0, 0.0);
	vec3 viewport_v = vec3(0.0, -viewport_height, 0.0);
	vec3 pixel_delta_u = viewport_u / static_cast<double>(image_width);
	vec3 pixel_delta_v = viewport_v / static_cast<double>(image_height);

	vec3 viewport_upper_left = camera_center - vec3(0.0, 0.0, focal_length) - viewport_u / 2.0 - viewport_v / 2.0;
	vec3 pixel100_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = 0; j < image_height; ++j)
	{
		std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i)
		{
			vec3 pixel_center = pixel100_loc + (static_cast<double>(i) * pixel_delta_u + static_cast<double>(j) * pixel_delta_v);
			vec3 ray_direction = pixel_center - camera_center;

			ray r(camera_center, ray_direction);

			color pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);
		}
	}

	return 0;
}