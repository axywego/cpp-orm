#pragma once

#define ORM_FIELDS_BEGIN(ClassName) \
    template<typename Func> \
    void for_each_column(Func&& func) { \
        auto& self = *static_cast<ClassName*>(this);

#define ORM_FIELD_ENTRY(field) \
        func(self.field);

#define ORM_FIELDS_END() \
    }
    