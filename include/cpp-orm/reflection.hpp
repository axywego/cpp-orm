#pragma once

#define ORM_BEGIN_FIELDS(ClassName) \
    template<typename Func> \
    void for_each_column(Func&& func) { \
        auto& self = *static_cast<ClassName*>(this);

#define ORM_FIELD(field) \
        func(self.field);

#define ORM_END_FIELDS() \
    }