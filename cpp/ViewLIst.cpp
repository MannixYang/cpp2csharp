#include <iostream>
#include <map>

struct View {
    int id;
};

class ViewList {
public:
    using MapType = std::map<int, View>;
    using Iterator = MapType::iterator;

    Iterator begin() { return views.begin(); }
    Iterator end()   { return views.end();   }

private:
    MapType views;
};


extern "C" {
// View
__declspec(dllexport) int View_getId(View* v) {
    return v->id;
}

// 容器
__declspec(dllexport) ViewList* ViewList_new() { return new ViewList(); }
__declspec(dllexport) void ViewList_delete(ViewList* v) { delete v; }

__declspec(dllexport) ViewList::Iterator* ViewList_begin(ViewList* v) {
    return new ViewList::Iterator(v->begin());
}
__declspec(dllexport) ViewList::Iterator* ViewList_end(ViewList* v) {
    return new ViewList::Iterator(v->end());
}
// 迭代器
__declspec(dllexport) void ViewIterator_delete(ViewList::Iterator* it) {
    delete it;
}
__declspec(dllexport) void ViewIterator_next(ViewList::Iterator* it) {
    ++(*it);
}
__declspec(dllexport) bool ViewIterator_equals(ViewList::Iterator* a, ViewList::Iterator* b) {
    return *a == *b;
}
__declspec(dllexport) int ViewIterator_key(ViewList::Iterator* it) {
    return (*it)->first;
}
__declspec(dllexport) View* ViewIterator_value(ViewList::Iterator* it) {
    return &((*it)->second);
}
}