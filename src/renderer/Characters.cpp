//
// File: Characters.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//

#include "renderer/Characters.h"
#include "include.h"
#include "math.h"

#include "GL/gl.h"
#include "GL/glu.h"

using namespace std;

cCharacterList * pCharacterList = NULL;


cCharacter::cCharacter ()
{
	m_id = 0;
	m_angle = 0.0f;
	m_body = 0;
	m_hue = 0;
	m_flag = 0;
	m_direction = 0;
	m_lastanim = 0;
	m_moving = false;
	
	m_name_change = false;
	m_sex = 0;
	m_gold = 0;
	m_armor = 0;
	m_weight = 0;
	
	m_strength = 0;
	m_dexterity = 0;
	m_intelligence = 0;
	
	m_hits_current = 0;
	m_hits_max = 0;
	m_stamina_current = 0;
	m_stamina_max = 0;
	m_mana_current = 0;
	m_mana_max = 0;
	
	setPosition(0, 0, 0);
	m_animtime = 0.0f;
	
	anim_frame = 0;
	
}

cCharacter::~cCharacter ()
{
	ClearEquip();
	ClearSkills();
}

void cCharacter::ClearEquip (void)
{
	equiplist_t::iterator iter;
	for (iter = equiplist.begin(); iter != equiplist.end(); iter++)
		delete iter->second;
	equiplist.clear();
}
   
cCharacterEquip * cCharacter::AddEquip (Uint32 layer)
{
	equiplist_t::iterator iter;
	iter = equiplist.find(layer);

	if (iter != equiplist.end()) 
		return iter->second; 

	cCharacterEquip * result = new cCharacterEquip;
	equiplist.insert(std::make_pair(layer, result));
	result->setLayer(layer);
	return result;
}

cCharacterEquip * cCharacter::GetEquip (unsigned int layer)
{
	equiplist_t::iterator iter;
	iter = equiplist.find(layer);

	if (iter != equiplist.end()) 
		return iter->second; 
	return NULL;
}


void cCharacter::MoveTo (float x, float y, float z)
{
	m_destx = x;
	m_desty = y;
	m_destz = z;
	m_moving = -1;
}

void cCharacter::Handle(float time_factor)
{
  time_factor *= 1.5f;
  float dest_angle = m_direction * 45.0f;
  if (m_angle != dest_angle) {
  	float delta_angle = m_angle - dest_angle;
	if (delta_angle >= 180.0f) delta_angle -= 360.0f;
	if (delta_angle < -180.0f) delta_angle += 360.0f;
	if (fabs(delta_angle) < time_factor * 500.0f) {
		m_angle = dest_angle;
	} else {
		if (delta_angle > 0.0f)
			m_angle -= time_factor * 500.0f; else	
			m_angle += time_factor * 500.0f;
		if (m_angle >= 360.0f) m_angle -= 360.0f;
		if (m_angle < 0.0f) m_angle += 360.0f;
	}  
  }

	
  if (!time_factor)
  	time_factor = 0.01f;
  
  if (!m_moving)
  	return;
  
  float dx = m_destx - m_x;
  float dy = m_desty - m_y;
  float dz = (m_destz - m_z) * 0.1f;

  float distance = sqrt(dx * dx + dy * dy + dz * dz);

  if((distance < time_factor * 5.0f)) {
    setPosition(m_destx, m_desty, m_destz);
    m_moving = 0;
  } else {
    dx /= distance;
    dy /= distance;
    dz /= distance;

    m_x += dx * time_factor * 5.0;
    m_y += dy * time_factor * 5.0;
    m_z += dz * time_factor * 50.0;
  }
}

sSkillEntry * cCharacter::skill (Uint16 id)
{
	skilllist_t::iterator iter;
	iter = m_skills.find (id);
	if (iter != m_skills.end()) 
		return iter->second;
	sSkillEntry * result = new sSkillEntry;
	memset (result, 0, sizeof (struct sSkillEntry));
	m_skills.insert(make_pair(id, result));
	return result;
	
}

void cCharacter::ClearSkills ()
{
	skilllist_t::iterator iter;
	for (iter = m_skills.begin(); iter != m_skills.end(); iter++) 
		delete iter->second;
	m_skills.clear();
	
}

 cCharacterList::cCharacterList ()
{
}


cCharacterList::~cCharacterList ()
{
	Clear();
}


void cCharacterList::Clear()
{
	characterlist_t::iterator iter;
	for (iter = characterlist.begin(); iter != characterlist.end(); iter++)
		delete iter->second;
	characterlist.clear();
}


cCharacter * cCharacterList::Add (Uint32 id) 
{
	characterlist_t::iterator iter;
	iter = characterlist.find(id);

	if (iter != characterlist.end()) 
		return iter->second; 

	cCharacter * result = new cCharacter;
	characterlist.insert(std::make_pair(id, result));
	result->setID(id);
	return result;
}
 

cCharacter * cCharacterList::Get (unsigned int id)
{
	characterlist_t::iterator iter;
	iter = characterlist.find(id);

	if (iter != characterlist.end()) 
		return iter->second; 
	else
		return NULL;
}

void cCharacterList::Handle(float time_factor)
{
	characterlist_t::iterator iter;
	for (iter = characterlist.begin(); iter != characterlist.end(); iter++)
		iter->second->Handle(time_factor);
}

void cCharacterList::Delete(unsigned int id)
{
	delete Get(id);
	characterlist.erase(id);
}

