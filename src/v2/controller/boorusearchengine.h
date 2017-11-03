#ifndef BOORUSEARCHENGINE_H
#define BOORUSEARCHENGINE_H

#include <iostream>

#include "connectionmanager.h"

#include "../model/boorupicture.h"
#include "../model/boorusite.h"
#include "../utils/json.h"
#include "../utils/output.h"

enum Ratings{
    RATING_ALL = 0,
    RATING_SAFE,
    RATING_QUESTIONNABLE,
    RATING_EXPLICIT,
    RATING_LIST_END
};

using namespace std;

class BooruSearchEngine
{
public:
    BooruSearchEngine(BooruSite* parent);

    QUrlQuery* generateRequest(string tags, int page, int limit);

    vector<BooruPicture*> search(string tags);
    vector<BooruPicture*> search(int page);
    vector<BooruPicture*> search(string tags, int page);
    vector<BooruPicture*> search(string tags, int page, int limit);
    vector<BooruPicture*> search();

    void set(string tags);
    void set(int page);
    void set(string tags, int page);
    void set(string tags, int page, int limit);

    vector<BooruPicture*> parse(QString data);

private:
    string tags;
    int page;
    int limit;
    BooruSite* parent;
};

#endif // BOORUSEARCHENGINE_H