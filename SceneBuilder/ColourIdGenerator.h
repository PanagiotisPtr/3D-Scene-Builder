#ifndef COLOUR_ID_GENERATOR_H
#define COLOUR_ID_GENERATOR_H

class ColourIdGenerator {
public:
	constexpr static float stepSize = 0.01f;
	static unsigned prevId;

	struct ColourId {
		float r, g, b;
	};

	static ColourId getColourId();

	static unsigned decodeId(const ColourId& ci);
private:
};

#endif