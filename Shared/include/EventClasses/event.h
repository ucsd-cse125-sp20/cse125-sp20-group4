#pragma once
#include <memory>
#include <string>

#include "ObjectClasses/object.h"

class Event {

    private:
    const std::string objectId;

    public:
    Event( const std::string & objectId );

    const std::string & getObjectId() const;
    virtual std::string serialize() const;
    virtual void apply( std::shared_ptr<Object> object ) const = 0;

};