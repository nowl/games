#ifndef __FONT_CACHE_H__
#define __FONT_CACHE_H__

#include <boost/noncopyable.hpp>

#include <map>
#include <sstream>

#include <SDL_ttf.h>

#include "DefException.h"
#include "Log.h"

class FontCache : private boost::noncopyable {
public:

	~FontCache() {
		//std::map<std::string, TTF_Font*>::iterator iter;
		//iter = mResources.begin();
		//for(; iter != mResources.end(); iter++)
		//	TTF_CloseFont(iter->second);
	}

    TTF_Font* get(const std::string& fontName, int fontSize) {
		std::stringstream lookup;
		lookup << fontName << " " << fontSize;
		std::string lookupStr = lookup.str();
		
        if(mResources.count(lookupStr) == 0)
            _loadFont(lookupStr, fontName, fontSize);

        return mResources[lookupStr];
    }

private:
    std::map<std::string, TTF_Font*> mResources;

    void _loadFont(const std::string& lookup, 
				   const std::string& name,
				   int size)
	{
        Log *log = Log::getLogger("GraphicsCache");
		std::stringstream message;
		message << "loading and caching \"" << name << "\" at size " << size;
		log->info(message.str());

		TTF_Font *font = TTF_OpenFont(name.c_str(), size);
		if(!font)
			throw DefException("Failed to load font: " + lookup);

		mResources[lookup] = font;
	}

};

extern FontCache GlobalFontCache;

#endif  //__FONT_CACHE_H__

