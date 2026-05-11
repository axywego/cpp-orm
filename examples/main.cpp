#include <orm/orm.hpp>
#include <iostream>

namespace schema {
    inline constexpr auto TABLE = "users";
    inline constexpr auto ID = "id";
    inline constexpr auto NAME = "full_name";
    inline constexpr auto EMAIL = "email";
    inline constexpr auto AGE = "age";
}

class User : public orm::EntityBase<User, orm::Entity<schema::TABLE>> {
public:
    orm::Column<int, 
        orm::PrimaryKey, 
        orm::AutoIncrement,
        orm::ColumnName<schema::ID>
    > id;
    
    orm::Column<std::string, 
        orm::Varchar<150>, 
        orm::NotNull,
        orm::ColumnName<schema::NAME>
    > fullName;
    
    orm::Column<std::string, 
        orm::Varchar<255>, 
        orm::NotNull, 
        orm::Unique,
        orm::ColumnName<schema::EMAIL>
    > email;
    
    orm::Column<int, 
        orm::Default<18>,
        orm::ColumnName<schema::AGE>
    > age;
    
    ORM_FIELDS_BEGIN(User)
        ORM_FIELD_ENTRY(id)
        ORM_FIELD_ENTRY(fullName)
        ORM_FIELD_ENTRY(email)
        ORM_FIELD_ENTRY(age)
    ORM_FIELDS_END()
};

int main() {
    // 1. Коннектимся один раз
    orm::Database::connect("dbname=test user=postgres password=secret host=localhost");
    
    std::cout << "🚀 Starting ORM demo...\n\n";
    
    // 2. Создаём таблицу
    User::create_table();
    std::cout << "-> Table created\n";
    
    // 3. Создаём юзеров
    User alice{0, "Alice Johnson", "alice@example.com", 30};
    alice.insert();
    std::cout << "-> Alice inserted\n";
    
    User bob{0, "Bob Smith", "bob@example.com", 25};
    bob.insert();
    std::cout << "-> Bob inserted\n";
    
    User carol{0, "Carol Williams", "carol@example.com", 35};
    carol.insert();
    std::cout << "-> Carol inserted\n\n";
    
    // 4. Выбираем всех
    std::cout << "📋 All users:\n";
    auto all_users = User::all();
    for (const auto& user : all_users) {
        std::cout << "  " << user.fullName << " (" << user.age << ") - " << user.email << "\n";
    }
    std::cout << "\n";
    
    // 5. Фильтруем
    std::cout << "🔍 Users older than 25:\n";
    auto older = User::find("age > 25");
    for (const auto& user : older) {
        std::cout << "  " << user.fullName << " - " << user.age << " years old\n";
    }
    std::cout << "\n";
    
    // 6. Ищем по ID
    std::cout << "🔍 Finding user with ID=1:\n";
    auto found = User::find_by_id(1);
    if (!found.empty()) {
        std::cout << "  Found: " << found[0].fullName << "\n\n";
    }
    
    // 7. Обновляем
    alice.age = 31;
    alice.update();
    std::cout << "-> Alice updated (age: 30 -> 31)\n\n";
    
    // 8. Проверяем обновление
    auto alice_check = User::find_by_id(1);
    if (!alice_check.empty()) {
        std::cout << "  Alice new age: " << alice_check[0].age << "\n\n";
    }
    
    // 9. Удаляем
    bob.remove();
    std::cout << "-> Bob removed\n\n";
    
    // 10. Финальный список
    std::cout << "📋 Final users list:\n";
    auto final_users = User::all();
    for (const auto& user : final_users) {
        std::cout << "  " << user.id << ": " << user.fullName << " - " << user.email << "\n";
    }
    
    std::cout << "\n✅ Demo completed!\n";
    return 0;
}