package Function;

import Entity.Company;
import Connect.connect;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

/**
 * 企业用户的管理功能
 */
public class Company_Manage {

    //使用者
    //Company company=new Company();

    /**
     * 增加小区/房源信息
     * @param inf 要增加的信息
     * @param type 要增加到哪个表
     */
    public boolean Insert(Object[] inf, String type){
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                if(type=="neighborhood") {
                    ResultSet resultSet=statement.executeQuery("select* from neighborhood where nname="+inf[3]+" and ntype="+inf[5]);
                    if(resultSet.next()){
                        return false;
                    }
                    String sql = "insert into neighborhood values ( ";
                    for (int i = 0; i < inf.length; i++) {
                        sql = sql  + inf[i] + ",";
                    }
                    String longtitude=inf[0].toString().replace("'","");
                    String altitude=inf[1].toString().replace("'","");
                    sql=sql+"geometry::STGeomFromText('Point("+longtitude+" "+altitude+")',4326));";
                    statement.execute(sql);
                    return true;
                }
                else if(type=="house"){
                    ResultSet resultSet=statement.executeQuery("select max(id) from house");
                    int id = -1;
                    while(resultSet.next()){
                        id=resultSet.getInt(1);
                    }
                    String sql="insert into house values ( "+(id+1)+", ";
                    for(int i=0;i<inf.length;i++){
                        sql=sql+inf[i]+",";
                    }
                    sql=sql+"null)";
                    System.out.println(sql);
                    statement.execute(sql);
                    return true;
                }
                statement.close();
                connection.close();
            }
        } catch (SQLException throwables) {
            return false;
        }
        return false;
    }

    /**
     * 删除小区/房源信息
     * @param inf 要删除的元组  小区为小区名+小区类型  房源为房源id
     */
    public boolean Delete(Object[] inf){
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                if(inf.length==1){//删除房源
                    ResultSet resultSet=statement.executeQuery("select* from house where id="+inf[0]);
                    if(!resultSet.next()){
                        return false;
                    }
                    String sql="delete from house where id="+inf[0];
                    statement.execute(sql);
                    return true;
                    //System.out.println("删除成功！");
                }
                else if(inf.length==2){//删除小区
                    ResultSet resultSet=statement.executeQuery("select* from neighborhood where nname='"+inf[0]+"' and ntype='"+inf[1]+"'");
                    if(!resultSet.next()){
                        return false;
                    }
                    String sql="delete from neighborhood where nname='"+inf[0]+"' and ntype='"+inf[1]+"'";
                    statement.execute(sql);
                    return true;
                    //System.out.println("删除成功！");
                }
                statement.close();
                connection.close();
            }
        } catch (SQLException throwables) {
            //System.out.println("删除失败！");
            throwables.printStackTrace();
        }
        return false;
    }

    /**
     * 修改小区/房源信息
     * @param inf 要更改的元组 小区为小区名+户型  房源为房源id
     * @param detail 要更改的具体信息
     */
    public boolean Update(Object[] inf,Object[] detail){
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                if(inf.length==1){//更改房源信息
                    ResultSet resultSet=statement.executeQuery("select id from house where id="+inf[0]);
                    if(resultSet.next()){
                        int id_before=resultSet.getInt(1);
                        String sql="update house set\n" +
                                //"id="+detail[0]+",\n" +
                                "house_type="+detail[0]+",\n" +
                                "[price(10000)]="+detail[1]+",\n" +
                                "[avg_price(yuan/square meter)]="+detail[2]+",\n" +
                                "[house_size(square meter)]="+detail[3]+",\n" +
                                "[inner_size(square meter)]="+detail[4]+",\n" +
                                "[towards]="+detail[5]+",\n" +
                                "[nname]="+detail[6]+",\n" +
                                "[hfloor]="+detail[7]+",\n" +
                                "[tfloor]="+detail[8]+",\n" +
                                "[build_year]="+detail[9]+",\n" +
                                "[build_type]="+detail[10]+"\n" +
                                "where id="+id_before;
                        statement.execute(sql);
                        //System.out.println("更改成功！");
                        statement.close();
                        connection.close();
                        return true;
                    }
                    else{
                        //System.out.println("找不到该房源！");
                        statement.close();
                        connection.close();
                        return false;
                    }
                }
                else if(inf.length==2){//更改小区信息
                    ResultSet resultSet=statement.executeQuery("select nname,ntype from neighborhood where nname='"+inf[0]+"' and ntype='"+inf[1]+"'");
                    if(resultSet.next()){
                        String nname_before=resultSet.getString(1);
                        String ntype_before=resultSet.getString(2);
                        String longitude=detail[0].toString().replace("'","");
                        String altitude=detail[1].toString().replace("'","");
                        String sql="update neighborhood set\n" +
                                "[longitude]="+detail[0]+",\n" +
                                "[altitude]="+detail[1]+",\n" +
                                "[area]="+detail[2]+",\n" +
                                "[nname]="+detail[3]+",\n" +
                                "[ntype]="+detail[4]+",\n" +
                                "[sales]="+detail[5]+",\n" +
                                "[price]="+detail[6]+",\n" +
                                "[plot]="+detail[7]+",\n" +
                                "[green]="+detail[8]+",\n" +
                                "[geom]=geometry::STGeomFromText('POINT ("+longitude+" "+altitude+")', 0)\n" +
                                "where [nname]='"+nname_before+"' and [ntype]='"+ntype_before+"'";
                        statement.execute(sql);
                        //System.out.println("更改成功！");
                        statement.close();
                        connection.close();
                        return true;
                    }
                    else{
                        //System.out.println("找不到该小区！");
                        statement.close();
                        connection.close();
                        return false;
                    }
                }
            }
        } catch (SQLException throwables) {
            //System.out.println("更改失败！");
            throwables.printStackTrace();
        }
        return false;
    }
}
