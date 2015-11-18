#pragma once

// TODO: Verify
class simulation_entity_definition
{
protected:
	~simulation_entity_definition() {}

public:
	virtual int getId();
	virtual char* getName();
};