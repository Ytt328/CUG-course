package Entity;

import Connect.connect;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

//管理员用户
public class Admin {
    String name;//用户名
    String password;//密码

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    /**
     * 删除用户
     * @param name 用户名
     * @return 删除成功与否
     */
    public boolean DeleteUser(String name){
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet=statement.executeQuery("select* from h_user where username='"+name+"'");
                if(resultSet.next()){
                    statement.execute("delete from h_user where username='"+name+"'");
                    statement.close();
                    connection.close();
                    return true;
                }
                else{
                    statement.close();
                    connection.close();
                    return false;
                }
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return false;
    }

    /**
     * 删除企业用户
     * @param name 用户名
     * @return 删除成功与否
     */
    public boolean DeleteCompany(String name){
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet=statement.executeQuery("select* from h_company where username='"+name+"'");
                if(resultSet.next()){
                    statement.execute("delete from h_company where username='"+name+"'");
                    statement.close();
                    connection.close();
                    return true;
                }
                else{
                    statement.close();
                    connection.close();
                    return false;
                }
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return false;
    }

    /**
     * 添加用户
     * @param name  用户名
     * @param password  用户密码
     * @return 添加成功与否
     */
    public boolean InsertUser(String name,String password){
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet=statement.executeQuery("select* from h_user where username='"+name+"'");
                if(resultSet.next()){
                    statement.close();
                    connection.close();
                    return false;
                }
                else{
                    statement.execute("insert into h_user(username,password) values ('"+name+"','"+password+"')");
                    statement.close();
                    connection.close();
                    return true;
                }
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return false;
    }

    /**
     * 修改用户信息
     * @param namebefore 修改前的用户名
     * @param name 修改后的用户名
     * @param password 修改后的密码
     * @return 修改成功与否
     */
    public boolean UpdateUser(String namebefore,String name,String password){
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet=statement.executeQuery("select* from h_user where username='"+namebefore+"'");
                if(resultSet.next()){
                    statement.execute("update h_user set username='" + name + "',password='" + password + "' where username='" + namebefore + "'");
                    statement.close();
                    connection.close();
                    return true;
                }
                else{
                    statement.close();
                    connection.close();
                    return false;
                }
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return false;
    }

    /**
     * 添加企业用户
     * @param name  用户名
     * @param password  用户密码
     * @return 添加成功与否
     */
    public boolean InsertCompany(String name,String password){
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet=statement.executeQuery("select* from h_company where username='"+name+"'");
                if(resultSet.next()){
                    statement.close();
                    connection.close();
                    return false;
                }
                else{
                    statement.execute("insert into h_company(username,password) values ('"+name+"','"+password+"')");
                    statement.close();
                    connection.close();
                    return true;
                }
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return false;
    }

    /**
     * 修改企业用户信息
     * @param namebefore 修改前的用户名
     * @param name 修改后的用户名
     * @param password 修改后的密码
     * @return 修改成功与否
     */
    public boolean UpdateCompany(String namebefore,String name,String password){
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet=statement.executeQuery("select* from h_company where username='"+namebefore+"'");
                if(resultSet.next()){
                    statement.execute("update h_company set username='" + name + "',password='" + password + "' where username='" + namebefore + "'");
                    statement.close();
                    connection.close();
                    return true;
                }
                else{
                    statement.close();
                    connection.close();
                    return false;
                }
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return false;
    }
}
