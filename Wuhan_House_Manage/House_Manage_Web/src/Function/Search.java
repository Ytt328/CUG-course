package Function;

import Connect.connect;
import Entity.*;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.*;

/**
 * 查询功能，所有用户均可使用
 */
public class Search {

    //使用者
    /*User user=new User();
    Company company=new Company();
    Admin admin=new Admin();*/

    /**
     * 查询条件整合,根据条件查询
     * @param filter 查询条件
     * @return 整合后的sql语句
     */
    public String Setsql(Object[] filter){
        String sql="select * from neighbor_house";
        String sql1 = null;
        if(filter[0]!=null&&filter[0]!=""){
            sql1="[area] ='"+filter[0]+"'";
        }
        String sql2 = null;
        if(filter[1]!=null&&filter[1]!=""){
            sql2="nname='"+filter[1]+"'";
        }

        String sql3 = null;
        Object nu=-1;
        if(filter[2]!=null&&filter[2]!=""){
            sql3="[price(10000)]>"+filter[2];
            if(filter[3]!=null&&filter[3]!=""){
                sql3=sql3+" and [price(10000)]<"+filter[3];
            }
        }
        else{
            if(filter[3]!=null&&filter[3]!=""){
                sql3="[price(10000)]<"+filter[3];
            }
        }

        String sql4=null;
        if(filter[4]!=null&&filter[4]!=""){
            sql4="house_type like '"+filter[4]+"室";
        }
        if(filter[5]!=null&&filter[5]!=""){
            sql4=(sql4==null)?("house_type like '%"+filter[5]+"厅"):(sql4+"%"+filter[5]+"厅");
        }
        if(filter[6]!=null&&filter[6]!=""){
            sql4=(sql4==null)?("house_type like '%"+filter[6]+"卫"):(sql4+"%"+filter[6]+"卫");
        }
        sql4=(sql4==null)?(null):(sql4+"%'");

        String sql5 = null;
        if(filter[7]!=null&&filter[7]!=""){
            sql5="[house_size(square meter)]>"+filter[7];
            if(filter[8]!=null&&filter[8]!=""){
                sql5=sql5+" and [house_size(square meter)]<"+filter[8];
            }
        }
        else{
            if(filter[8]!=null&&filter[8]!=""){
                sql5="[house_size(square meter)]<"+filter[8];
            }
        }

        String sql6=null;
        sql6=(sql1==null)?sql6:sql1;
        if(sql2!=null) {
            sql6=(sql6==null)?sql2:(sql6+" and "+sql2);
        }
        if(sql3!=null) {
            sql6=(sql6==null)?sql3:(sql6+" and "+sql3);
        }
        if(sql4!=null) {
            sql6=(sql6==null)?sql4:(sql6+" and "+sql4);
        }
        if(sql5!=null) {
            sql6=(sql6==null)?sql5:(sql6+" and "+sql5);
        }

        sql=(sql6==null)?sql:(sql+" where "+sql6);
        return sql;
    }

    /**
     * 获取数据总数,按条件查询
     * @param sql 查询条件整合的sql语句
     * @return 数据总数
     */
    public int Count1(String sql){
        int count=0;
        sql=sql.replace("*","count(*)");
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                //System.out.println(sql);
                ResultSet resultSet=statement.executeQuery(sql);
                while(resultSet.next()){
                    count=resultSet.getInt(1);
                    statement.close();
                    connection.close();
                    return count;
                }
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return count;
    }

    /**
     *按条件分页查询
     * @return 查询结果
     */
    public PageRequest SearchbyFilterPage(String sql, int currentpage){
        Connection connection=new connect().getconnet();
        PageRequest pageRequest=new PageRequest();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                int start=(currentpage-1)*10;

                String sql1="select distinct top(10)* from ("+sql+") as a where id not in (";
                sql=sql.replace("*"," top("+start+") id");
                sql1=sql1+sql+")";
                //System.out.println(sql1);
                ResultSet resultSet=statement.executeQuery(sql1);
                List<House> rs = new ArrayList<>();
                while(resultSet.next()){
                    House house=new House();
                    house.setID(resultSet.getInt(1));
                    house.setHousetype(resultSet.getString(2));
                    house.setPrice(resultSet.getString(3));
                    house.setAvgprice(resultSet.getString(4));
                    house.setSize(resultSet.getString(5));
                    house.setInnersize(resultSet.getString(6));
                    house.setTowards(resultSet.getString(7));
                    house.setNname(resultSet.getString(8));
                    house.setHfloor(resultSet.getString(9));
                    house.setTfloor(resultSet.getString(10));
                    house.setBuildyear(resultSet.getString(11));
                    house.setBuildtype(resultSet.getString(12));
                    house.setComment(resultSet.getString(13));
                    house.setArea(resultSet.getString(14));
                    rs.add(house);
                }
                pageRequest.setRs(rs);
                pageRequest.setCurrentpage(currentpage);
                statement.close();
                connection.close();
                return pageRequest;
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return pageRequest;
    }

    /**
     * 按小区名查询房源
     * @param nname 小区名
     * @return 查询结果
     */
    public List<House> Searchbynname(String nname){
        List<House> houseList=new ArrayList<>();
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet;
                resultSet=statement.executeQuery("select distinct * from neighbor_house where nname='"+nname+"'");
                while(resultSet.next()){
                    House house=new House();
                    house.setID(resultSet.getInt(1));
                    house.setHousetype(resultSet.getString(2));
                    house.setPrice(resultSet.getString(3));
                    house.setAvgprice(resultSet.getString(4));
                    house.setSize(resultSet.getString(5));
                    house.setInnersize(resultSet.getString(6));
                    house.setTowards(resultSet.getString(7));
                    house.setNname(resultSet.getString(8));
                    house.setHfloor(resultSet.getString(9));
                    house.setTfloor(resultSet.getString(10));
                    house.setBuildyear(resultSet.getString(11));
                    house.setBuildtype(resultSet.getString(12));
                    house.setComment(resultSet.getString(13));
                    house.setArea(resultSet.getString(14));
                    houseList.add(house);
                }
                statement.close();
                connection.close();
                return houseList;
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return houseList;
    }

    /**
     * 获取数据总数，按道路查询
     * @param rname 道路名
     * @param distance 距离范围
     * @return 数据总数
     */
    public int Count2(String rname,float distance){
        Connection connection=new connect().getconnet();
        int count=0;
        try {
            if(connection.getAutoCommit()) {
                Statement statement=connection.createStatement();
                String sql = "declare @geom1 geometry\n" +
                        "set @geom1=(select top 1geom from road where rname='" + rname + "')\n" +
                        "select count(*) from neighbor_house where nname in\n" +
                        "(SELECT nname FROM [neighborhood] WHERE \n" +
                        "geography::STGeomFromText(geom.ToString(), 4326).STDistance(\n" +
                        "geography::STGeomFromText(@geom1.ToString(), 4326))<" + distance + ")";
                ResultSet resultSet=statement.executeQuery(sql);
                while(resultSet.next()){
                    count=resultSet.getInt(1);
                    return count;
                }
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return count;
    }

    /**
     * 按道路分页查询
     * @param rname 道路名
     * @param distance 距离范围
     * @param currentpage 当前页号
     * @return 查询的房源结果
     */
    public PageRequest SearchbyRoadPage(String rname,float distance,int currentpage){
        PageRequest pageRequest=new PageRequest();
        int start=(currentpage-1)*10;
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()) {
                Statement statement=connection.createStatement();
                String sql = "declare @geom1 geometry\n" +
                        "set @geom1 = (select top 1geom from road where rname='" + rname + "')\n" +
                        "select distinct top(10)* from (\n" +
                        "select * from neighbor_house where nname in\n" +
                        "(SELECT nname FROM [neighborhood] WHERE\n" +
                        "geography::STGeomFromText(geom.ToString(), 4326).STDistance(\n" +
                        "geography::STGeomFromText(@geom1.ToString(), 4326))<" + distance + "))\n" +
                        "as a\n" +
                        "where id not in (\n" +
                        "    select  top(" + start + ") id from neighbor_house where nname in\n" +
                        "    (SELECT nname FROM [neighborhood] WHERE\n" +
                        "    geography::STGeomFromText(geom.ToString(), 4326).STDistance(\n" +
                        "    geography::STGeomFromText(@geom1.ToString(), 4326))<" + distance + "))";
                ResultSet resultSet=statement.executeQuery(sql);
                List<House> rs = new ArrayList<>();
                while(resultSet.next()){
                    House house=new House();
                    house.setID(resultSet.getInt(1));
                    house.setHousetype(resultSet.getString(2));
                    house.setPrice(resultSet.getString(3));
                    house.setAvgprice(resultSet.getString(4));
                    house.setSize(resultSet.getString(5));
                    house.setInnersize(resultSet.getString(6));
                    house.setTowards(resultSet.getString(7));
                    house.setNname(resultSet.getString(8));
                    house.setHfloor(resultSet.getString(9));
                    house.setTfloor(resultSet.getString(10));
                    house.setBuildyear(resultSet.getString(11));
                    house.setBuildtype(resultSet.getString(12));
                    house.setComment(resultSet.getString(13));
                    house.setArea(resultSet.getString(14));
                    rs.add(house);
                }
                pageRequest.setRs(rs);
                pageRequest.setCurrentpage(currentpage);
                statement.close();
                connection.close();
                return pageRequest;

            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return pageRequest;
    }

    /**
     * 通过ID查找房源
     * @param id 房源ID
     * @return 找到的房源信息
     */
    public House SearchHousebyID(int id){
        Connection connection=new connect().getconnet();
        House house=new House();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet=statement.executeQuery("select* from neighbor_house where id="+id);
                if(resultSet.next()){
                    house.setID(resultSet.getInt(1));
                    house.setHousetype(resultSet.getString(2));
                    house.setPrice(resultSet.getString(3));
                    house.setAvgprice(resultSet.getString(4));
                    house.setSize(resultSet.getString(5));
                    house.setInnersize(resultSet.getString(6));
                    house.setTowards(resultSet.getString(7));
                    house.setNname(resultSet.getString(8));
                    house.setHfloor(resultSet.getString(9));
                    house.setTfloor(resultSet.getString(10));
                    house.setBuildyear(resultSet.getString(11));
                    house.setBuildtype(resultSet.getString(12));
                    house.setComment(resultSet.getString(13));
                    house.setArea(resultSet.getString(14));
                    statement.close();
                    connection.close();
                    return house;
                }
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return house;
    }

    /**
     * 通过小区名查找小区
     * @param nname 小区名
     * @return 找到的小区信息
     */
    public Neigh SearchNeighbyNname(String nname){
        Connection connection=new connect().getconnet();
        Neigh neigh=new Neigh();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet=statement.executeQuery("select top 1* from neighborhood where nname='"+nname+"'");
                while (resultSet.next()){
                    neigh.setLongitude(resultSet.getFloat(1));
                    neigh.setAltitude(resultSet.getFloat(2));
                    neigh.setArea(resultSet.getString(3));
                    neigh.setNname(resultSet.getString(4));
                    neigh.setSales(resultSet.getString(5));
                    neigh.setNtype(resultSet.getString(6));
                    neigh.setPrice(resultSet.getFloat(7));
                    neigh.setPlot(resultSet.getFloat(8));
                    neigh.setGreen(resultSet.getFloat(9));
                    statement.close();
                    connection.close();
                    return neigh;
                }
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return neigh;
    }

    /**
     * 获取所有用户
     * @return 所有用户集合
     */
    public List<User> SearchUser(){
        List<User> userList=new ArrayList<>();
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet=statement.executeQuery("select username,password from h_user");
                while(resultSet.next()){
                    User user=new User();
                    user.setName(resultSet.getString(1));
                    user.setPassword(resultSet.getString(2));
                    userList.add(user);
                }
                statement.close();
                connection.close();
                return userList;
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return userList;
    }

    /**
     * 获取所有企业用户
     * @return 所有企业用户集合
     */
    public List<Company> SearchCompany(){
        List<Company> companyList=new ArrayList<>();
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet=statement.executeQuery("select username,password from h_company");
                while(resultSet.next()){
                    Company company=new Company();
                    company.setName(resultSet.getString(1));
                    company.setPassword(resultSet.getString(2));
                    companyList.add(company);
                }
                statement.close();
                connection.close();
                return companyList;
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return companyList;
    }

    /**
     * 通过用户名查找用户
     * @param name 用户名
     * @return 查找到的用户
     */
    public User SearchUserbyname(String name){
        User user=new User();
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet=statement.executeQuery("select username,password from h_user where username='"+name+"'");
                while(resultSet.next()){
                    user.setName(resultSet.getString(1));
                    user.setPassword(resultSet.getString(2));
                    statement.close();
                    connection.close();
                    return user;
                }
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return user;
    }

    /**
     * 通过用户名查找企业用户
     * @param name 用户名
     * @return 查找到的用户
     */
    public Company SearchCompanybyname(String name){
        Company company=new Company();
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet=statement.executeQuery("select username,password from h_company where username='"+name+"'");
                while(resultSet.next()){
                    company.setName(resultSet.getString(1));
                    company.setPassword(resultSet.getString(2));
                    statement.close();
                    connection.close();
                    return company;
                }
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return company;
    }

    /**
     * 查询小区附近1000米范围内的医院
     * @param nname 小区名
     * @return 查询结果
     */
    public List<Hospital> SearchSurroundHospital(String nname){
        List<Hospital> hospitalList=new ArrayList<>();
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet;
                String sql="declare @geom1 geometry\n" +
                        "set @geom1=(select top 1geom from neighborhood where nname='"+nname+"')\n" +
                        "select distinct hname from hospital where\n" +
                        "geography::STGeomFromText(geom.ToString(), 4326).STDistance(\n" +
                        "geography::STGeomFromText(@geom1.ToString(), 4326))<1000";
                resultSet=statement.executeQuery(sql);
                while(resultSet.next()){
                    Hospital hospital=new Hospital();
                    hospital.setHname(resultSet.getString(1));
                    hospitalList.add(hospital);
                }
                statement.close();
                connection.close();
                return hospitalList;
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return hospitalList;
    }

    /**
     * 查询小区附近1000米范围内的超市
     * @param nname 小区名
     * @return 查询结果
     */
    public List<Market> SearchSurroundMarket(String nname){
        List<Market> marketList=new ArrayList<>();
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet;
                String sql="declare @geom1 geometry\n" +
                        "set @geom1=(select top 1geom from neighborhood where nname='"+nname+"')\n" +
                        "select distinct mname from market where\n" +
                        "geography::STGeomFromText(geom.ToString(), 4326).STDistance(\n" +
                        "geography::STGeomFromText(@geom1.ToString(), 4326))<1000";
                resultSet=statement.executeQuery(sql);
                while(resultSet.next()){
                    Market market=new Market();
                    market.setMname(resultSet.getString(1));
                    marketList.add(market);
                }
                statement.close();
                connection.close();
                return marketList;
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return marketList;
    }


    /**
     * 查询小区附近1000米范围内的公园
     * @param nname 小区名
     * @return 查询结果
     */
    public List<Park> SearchSurroundPark(String nname){
        List<Park> parkList=new ArrayList<>();
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet;
                String sql="declare @geom1 geometry\n" +
                        "set @geom1=(select top 1geom from neighborhood where nname='"+nname+"')\n" +
                        "select distinct pname from park where\n" +
                        "geography::STGeomFromText(geom.ToString(), 4326).STDistance(\n" +
                        "geography::STGeomFromText(@geom1.ToString(), 4326))<1000";
                resultSet=statement.executeQuery(sql);
                while(resultSet.next()){
                    Park park=new Park();
                    park.setPname(resultSet.getString(1));
                    parkList.add(park);
                }
                statement.close();
                connection.close();
                return parkList;
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return parkList;
    }


    /**
     * 查询小区附近1000米范围内的学校
     * @param nname 小区名
     * @return 查询结果
     */
    public List<School> SearchSurroundSchool(String nname){
        List<School> schoolList=new ArrayList<>();
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet;
                String sql="declare @geom1 geometry\n" +
                        "set @geom1=(select top 1geom from neighborhood where nname='"+nname+"')\n" +
                        "select distinct sname from school where\n" +
                        "geography::STGeomFromText(geom.ToString(), 4326).STDistance(\n" +
                        "geography::STGeomFromText(@geom1.ToString(), 4326))<1000";
                resultSet=statement.executeQuery(sql);
                while(resultSet.next()){
                    School school=new School();
                    school.setSname(resultSet.getString(1));
                    schoolList.add(school);
                }
                statement.close();
                connection.close();
                return schoolList;
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return schoolList;
    }


    /**
     * 查询小区附近1000米范围内的道路
     * @param nname 小区名
     * @return 查询结果
     */
    public List<Road> SearchSurroundRoad(String nname){
        List<Road> roadList=new ArrayList<>();
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                ResultSet resultSet;
                String sql="declare @geom1 geometry\n" +
                        "set @geom1=(select top 1geom from neighborhood where nname='"+nname+"')\n" +
                        "select distinct rname from road where\n" +
                        "geography::STGeomFromText(geom.ToString(), 4326).STDistance(\n" +
                        "geography::STGeomFromText(@geom1.ToString(), 4326))<1000";
                resultSet=statement.executeQuery(sql);
                while(resultSet.next()){
                    Road road=new Road();
                    road.setRname(resultSet.getString(1));
                    roadList.add(road);
                }
                statement.close();
                connection.close();
                return roadList;
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return roadList;
    }

    /**
     * 查询所有小区
     * @return 查询结果
     */
    public List<Neigh> SearchNeigh(){
        List<Neigh> neighList=new ArrayList<>();
        Connection connection=new connect().getconnet();
        try {
            if(connection.getAutoCommit()){
                Statement statement=connection.createStatement();
                System.out.println(1);
                ResultSet resultSet=statement.executeQuery("select  longitude,altitude,nname from neighborhood");
                while (resultSet.next()){
                    Neigh neigh=new Neigh();
                    neigh.setLongitude(resultSet.getFloat(1));
                    neigh.setAltitude(resultSet.getFloat(2));
                    neigh.setNname(resultSet.getString(3));
                    neighList.add(neigh);
                    System.out.println(2);
                }
                System.out.println(3);
                statement.close();
                connection.close();
                return neighList;
            }
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        return neighList;
    }
}
