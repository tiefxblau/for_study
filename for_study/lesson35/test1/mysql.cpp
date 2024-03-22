#include <mysql/mysql.h>
#include <iostream>
#include <string>


const std::string host = "101.43.183.8";
const std::string usr = "illusR";
const std::string password = "ILLYy3142536_";
const std::string db = "dorm";
uint port = 3556;
int main()
{
    // std::cout << mysql_get_client_info() << std::endl;

    // mysql句柄初始化
    MYSQL* mysql = mysql_init(nullptr);

    // 设置charset
    mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "utf8");

    // 初始化完毕之后，必须先链接数据库，在进行后续操作
    if (!mysql_real_connect(mysql, host.c_str(), usr.c_str(), password.c_str(), db.c_str(), port, nullptr, 0))
    {
        std::cout << "connect failed" << std::endl;
        return 1;
    }
    std::cout << "connect success" << std::endl;

    while (true)
    {
        // 下发mysql命令query
        std::string query;
        std::getline(std::cin, query);
        if (mysql_query(mysql, query.c_str()) != 0)
        {
            std::cout << "query failed" << std::endl;
        }

        if (query.find("select") != std::string::npos)
        {
            std::cout << "select" << std::endl; 

            // 获取执行结果mysql_store_result
            MYSQL_RES* res = mysql_store_result(mysql);

            // 获取结果行数mysql_num_rows
            my_ulonglong row = mysql_num_rows(res);

            // 获取结果列数mysql_num_fields
            uint col = mysql_num_fields(res);

            // 获取列名mysql_fetch_fields
            MYSQL_FIELD* field = mysql_fetch_field(res);
            for (int i = 0; i < col; ++i)
            {
                std::cout << field[i].name << "\t";
            }
            std::cout << std::endl;

            // 按行获取结果内容mysql_fetch_row
            for (int i = 0; i < row; ++i)
            {
                MYSQL_ROW line = mysql_fetch_row(res);
                for (int j = 0; j < col; ++j)
                {
                    std::cout << line[j] << "\t";
                }
                std::cout << std::endl;
            }
        }
    }

    // 关闭mysql链接mysql_close
    mysql_close(mysql);

    return 0;
}