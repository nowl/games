#ifndef __GRAPHICS_ASSET_H__
#define __GRAPHICS_ASSET_H__

#include <cstdio>
#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>

#include <boost/any.hpp>

#include "Log.h"

namespace fs = boost::filesystem;

class GraphicsAsset : private boost::noncopyable {
public:
    GraphicsAsset() : mValidAsset(false), mContentsLen(0) {}

    GraphicsAsset(const std::string& name)
        : mName(name), mContentsLen(0)
    {
        Log *log = Log::getLogger("GraphicsAsset");

        try {
            mContentsLen = fs::file_size(name);
        } catch(fs::basic_filesystem_error< fs::basic_path<std::string,  fs::path_traits> >& e) {
            log->warn(std::string("missing file: ") + name);
            mValidAsset = false;
            return;
        }

        mValidAsset = true;

        _readContents(name.c_str());
    }

    virtual ~GraphicsAsset() {
        if(mValidAsset)
            delete [] mContents;
    }

    const std::string &getName() const { return mName; }
    virtual boost::any getContents() { 
       return mContents;
    }
    int getContentsLen() const { return mContentsLen; }

protected:
    bool mValidAsset;
    std::string mName;

private:
    unsigned char *mContents;
    int mContentsLen;

    void _readContents(const char *fn) {
        FILE *fin = fopen(fn, "rb");

        mContents = new unsigned char[mContentsLen];

        char c = getc(fin);
        int i = 0;
        while(c != EOF) {
            mContents[i++] = c;
            c = getc(fin);
        }

        fclose(fin);
    }
};

#endif  //__GRAPHICS_ASSET_H__

