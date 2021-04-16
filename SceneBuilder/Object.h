#ifndef OBJECT_OBJECT_H
#define OBJECT_OBJECT_H

#include <functional>

#include "Globals.h"
#include "ColourIdGenerator.h"

class Object {
public:
	Object() : colourId(ColourIdGenerator::getColourId()), objectId(ColourIdGenerator::prevId), selected(false) {}

	virtual void draw() const = 0;
	virtual void drawWithId() const = 0;

	unsigned getObjectId() const {
		return this->objectId;
	}

	void toggleSelect() { selected = !selected; }
	void select() { selected = true; }
	void deselect() { selected = false; }
protected:
	ColourIdGenerator::ColourId colourId;
	unsigned objectId;
	bool selected;
};

#endif