#include "stdafx.h"
#include "physics.h"

std::vector<ContactInfo> Physics::CollideWithWorld(const glm::vec3& spherePos, float sphereRad) const 
{
	std::vector<ContactInfo> contacts;

	for (auto c : staticColliders) 
	{
		glm::vec3 normal;
		float depth;
		if (c->Collide(spherePos, sphereRad, normal, depth)) 
		{
			contacts.push_back(ContactInfo{ normal, depth});
		}
	}

	return contacts;
}

void Physics::Update(float deltaTime) 
{
	for (auto p : projectiles)
	{
		// updates position and velocity of projectiles
		p->pos += p->vel * deltaTime;
		p->vel += gravity * deltaTime;
		std::vector<ContactInfo> contacts = CollideWithWorld( p->pos, 0.2f);
		for (auto c : contacts)
		{
			// check for collision
			if ( glm::dot( p->vel, c.normal) <0.0f) 
			{
				p->pos += c.normal * c.depth;
				p->vel -= c.normal * glm::dot(c.normal, p->vel) * 1.5f;
			}
		}
	}
}