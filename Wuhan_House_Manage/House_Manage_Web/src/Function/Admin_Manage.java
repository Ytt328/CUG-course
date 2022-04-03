package Function;

import Entity.Admin;
import Connect.connect;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

/**
 * 管理员的管理功能
 */
public class Admin_Manage {

    //使用者
    //Admin admin=new Admin();

    /**增加普通/企业用户
     * @param name 用户名
     * @param password 密码
     * @param type 用户类型 (普通/企业用户)
     * @return 增加成功与否
     */
    public boolean InsertUser(String name,String password,String type){
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                String sql="select* from "+type+"where username='"+name+"';";
                ResultSet resultSet=statement.executeQuery(sql);
                if(resultSet.next()){
                    System.out.println("增加失败！");
                    return false;
                }
                else{
                    sql="insert into "+type+"(username,password) values('"+name+"','"+password+"');";
                    System.out.println("增加成功！");
                    return true;
                }
            }
        } catch (SQLException throwables) {
            System.out.println("增加失败！");
            throwables.printStackTrace();
        }
        return false;
    }

    /**删除普通/企业用户
     * @param name 用户名
     * @param type 用户类型(普通/企业用户)
     * @return 删除成功与否
     */
    public boolean DeleteUser(String name,String type){
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                String sql="delete from "+type+" where username='"+name+"';";
                statement.execute(sql);
                System.out.println("删除成功！");
                return true;
            }
        } catch (SQLException throwables) {
            System.out.println("删除失败！");
            throwables.printStackTrace();
        }
        return false;
    }

    /**修改普通/企业用户
     * @param name_before 原用户名
     * @param name 更改后的用户名
     * @param password 更改后的密码
     * @param type 用户类型(普通/企业用户)
     * @return 修改成功与否
     */
    public boolean UpdateUser(String name_before,String name,String password,String type){
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                String sql="select* from "+type+" where username='"+name_before+"';";
                ResultSet resultSet=statement.executeQuery(sql);
                if(resultSet.next()){
                    sql="select* from "+type+" where username='"+name+"'";
                    resultSet=statement.executeQuery(sql);
                    if(resultSet.next()){
                        sql="insert into "+type+"(username,password) values('"+name+"','"+password+"');";
                        statement.execute(sql);
                        System.out.println("修改成功！");
                        return true;
                    }
                    else{
                        System.out.println("用户已存在！请尝试更改用户名！");
                        return false;
                    }
                }
                else{
                    System.out.println("找不到该用户！修改失败");
                    return false;
                }
            }
        } catch (SQLException throwables) {
            System.out.println("修改失败！");
            throwables.printStackTrace();
        }
        return false;
    }

}
