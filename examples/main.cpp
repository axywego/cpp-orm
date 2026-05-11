#include <orm/core/entity_base.hpp>
#include <orm/drivers/postgres.hpp>
#include <iostream>

inline constexpr char USERS_TABLE[] = "users";
inline constexpr char COL_ID[] = "id";
inline constexpr char COL_NAME[] = "full_name";
inline constexpr char COL_EMAIL[] = "email";
inline constexpr char COL_AGE[] = "age";

class User : public orm::EntityBase<User, orm::Entity<USERS_TABLE>> {
public:
    orm::Column<int, 
        orm::PrimaryKey, 
        orm::AutoIncrement,
        orm::ColumnName<COL_ID>
    > id;
    
    orm::Column<std::string, 
        orm::Varchar<150>, 
        orm::NotNull,
        orm::ColumnName<COL_NAME>
    > fullName;
    
    orm::Column<std::string, 
        orm::Varchar<255>, 
        orm::NotNull, 
        orm::Unique,
        orm::ColumnName<COL_EMAIL>
    > email;
    
    orm::Column<int, 
        orm::Default<18>,
        orm::ColumnName<COL_AGE>
    > age;
    
    ORM_FIELDS_BEGIN(User)
        ORM_FIELD_ENTRY(id)
        ORM_FIELD_ENTRY(fullName)
        ORM_FIELD_ENTRY(email)
        ORM_FIELD_ENTRY(age)
    ORM_FIELDS_END()
};

int main() {
    orm::PostgresConnection conn("dbname=test user=postgres password=secret");
    
    conn.execute(User::create_table_sql());
    
    User user{0, "Alice", 30};
    user.insert(conn);
    
    auto users = User::find(conn, "age > 25");
    for (const auto& u : users) {
        std::cout << u.name << std::endl;
    }
    
    return 0;
}