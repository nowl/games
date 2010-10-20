#ifndef __GRAPHICS_CACHE_H__
#define __GRAPHICS_CACHE_H__

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <map>
#include <iostream>

#include "ImageGraphicsAsset.h"
#include "GraphicsAsset.h"
#include "Log.h"

class GraphicsCache : private boost::noncopyable {
public:
    enum { GRAPHICS_ASSET,
           IMAGE_GRAPHICS_ASSET };
    
    GraphicsAsset* get(const std::string& resourceName, int type = GRAPHICS_ASSET) {
        if(mResources.count(resourceName) == 0)
            _loadResource(resourceName, type);

        return mResources[resourceName].get();
    }

    int getCount() const {
        return mResources.size();
    }
    
private:
    std::map<std::string, boost::shared_ptr<GraphicsAsset> > mResources;

    void _loadResource(const std::string& name, int type) {
        Log *log = Log::getLogger("GraphicsCache");

        log->info("loading and caching \"" + name + "\"");

        switch(type) {
        case GRAPHICS_ASSET:
            mResources[name] = boost::shared_ptr<GraphicsAsset>(new GraphicsAsset(name));
            break;
        case IMAGE_GRAPHICS_ASSET:
            mResources[name] = boost::shared_ptr<GraphicsAsset>(new ImageGraphicsAsset(name));
            break;
        default:
            mResources[name] = boost::shared_ptr<GraphicsAsset>(new GraphicsAsset(name));
            break;
        }
    }
};

extern GraphicsCache GlobalGraphicsCache;

#endif  //__GRAPHICS_CACHE_H__

