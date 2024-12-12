#pragma once
#include "MapObject.h"
#include "string"

class Gate final : protected MapObject {

private:
	MapLayer* parent;
	std::string NextMap;
	std::string NextPosition;

public:
	static MapObject* create(rapidjson::Value& val, MapLayer* parent, const Vec<int>& pos);

	// tp
	virtual void interact() override;



	explicit Gate(MapLayer* parent, const Vec<int>& pos, std::string NM, std::string NP);
	~Gate() override;
	
	virtual void init() override;
	virtual void clear() override;
	virtual void pause() override;
	virtual void resume() override;
	virtual void settle() override;
	virtual bool hasCollision() override;
};

