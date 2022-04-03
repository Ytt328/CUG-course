package Entity;

import Connect.connect;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

//普通游客用户
public class User {
    String name;//用户名
    String password;//用户密码


    public String getName() {
        return name;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public void setName(String name){
        this.name=name;
    }


    /**
     * 评论房源，只有游客可使用
     * @param id  要评论的房源id
     * @param detail  评论的内容
     */
    public void Comment(int id,String detail){
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                //加入到房源表中
                String sql="select comment from house where id="+id;
                ResultSet resultSet=statement.executeQuery(sql);
                if(name==null){
                    System.out.println("未登录！无法评论");
                    statement.close();
                    connection.close();
                    return;
                }
                String detail_before=null;
                if(resultSet.next()) {
                    detail_before = resultSet.getString(1);
                }
                String detail_now=(detail_before==null)?"用户"+name+":"+detail:detail_before+"\n"+"用户"+name+":"+detail;
                String sql1="update house set comment='"+detail_now+"' where id="+id;
                statement.execute(sql1);

                //加入到用户表中
                String sql2="select comment from h_user where username='"+name+"';";
                ResultSet resultSet1=statement.executeQuery(sql2);
                String detail_before1=null;
                while(resultSet1.next()){
                    detail_before1=resultSet1.getString(1);
                }
                String detail_now1=(detail_before1==null)?"房源"+id+":"+detail:detail_before1+"\n"+"房源"+id+":"+detail;
                String sql3="update h_user set comment='"+detail_now1+"' where username='"+name+"';";
                statement.execute(sql3);
                statement.close();
                connection.close();
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
    }

}
