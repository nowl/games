#include "Event.h"

using namespace std;
using namespace boost;

Event::Event(EventType type, const std::vector< boost::any >& data)
	: _type(type), _data(data)
{}

MouseButtonEventWrapper::MouseButtonEventWrapper(const IEvent& event)
{
	// TODO: throw exception if event not Mouse event

	vector< any > data = event.getData();
	_button = any_cast<Event::MouseButton>(data[0]);
	_x = any_cast<unsigned int>(data[1]);
	_y = any_cast<unsigned int>(data[2]);
}

MouseMoveEventWrapper::MouseMoveEventWrapper(const IEvent& event)
{
	// TODO: throw exception if event not Mouse event

	vector< any > data = event.getData();
	_x = any_cast<unsigned int>(data[0]);
	_y = any_cast<unsigned int>(data[1]);
	_xrel = any_cast<unsigned int>(data[2]);
	_yrel = any_cast<unsigned int>(data[3]);
}
