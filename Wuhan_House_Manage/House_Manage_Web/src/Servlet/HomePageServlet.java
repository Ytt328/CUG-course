package Servlet;

import Entity.*;
import Function.Company_Manage;
import Function.Search;

import javax.servlet.*;
import javax.servlet.http.*;
import javax.servlet.annotation.*;
import java.io.IOException;
import java.util.List;

/**
 * 主页上相关的操作
 */

@WebServlet(name = "HomepageServlet", value = "/HomepageServlet")
public class HomePageServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        doPost(request,response);
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        request.setCharacterEncoding("UTF-8");
        response.setCharacterEncoding("UTF-8");
        response.setContentType("text/html");

        //按条件查询
        if(request.getParameter("type").equals("searchbyfilter")) {
            request.setAttribute("type",request.getParameter("type"));
            Object[] filter = new Object[9];
            filter[0] = request.getParameter("xz");
            filter[1] = request.getParameter("neigh");
            filter[2] = request.getParameter("min_price");
            filter[3] = request.getParameter("max_price");
            filter[4] = request.getParameter("shi");
            filter[5] = request.getParameter("ting");
            filter[6] = request.getParameter("wei");
            filter[7] = request.getParameter("min_size");
            filter[8] = request.getParameter("max_size");
            request.getSession().setAttribute("filter",filter);
            String PagenumStr = request.getParameter("pagenum");

            int Pagenum;
            if (PagenumStr == null||PagenumStr.equals("")) {
                Pagenum = 1;
            } else {
                Pagenum = Integer.parseInt(PagenumStr);
            }

            String sql=new Search().Setsql(filter);
            int countnum=new Search().Count1(sql);
            int countpage;
            if(countnum%10>0){
                countpage=(countnum/10)+1;
            }
            else{
                countpage=(countnum/10);
            }
            PageRequest pageRequest =new Search().SearchbyFilterPage(sql,Pagenum);
            pageRequest.setTotalcount(countnum);
            pageRequest.setTotalpage(countpage);
            pageRequest.setPre(Pagenum==1?1:Pagenum-1);
            pageRequest.setNext(Pagenum==countpage?countpage:Pagenum+1);
            if (request.getSession().getAttribute("type").equals("user")) {
                request.setAttribute("result", pageRequest);
                request.getRequestDispatcher("User.jsp").forward(request, response);
            } else if (request.getSession().getAttribute("type").equals("company")) {
                request.setAttribute("result", pageRequest);
                request.getRequestDispatcher("Company.jsp").forward(request, response);
            } else {
                request.setAttribute("result", pageRequest);
                request.getRequestDispatcher("Admin.jsp").forward(request, response);
            }
        }
        //按道路查询
        else if(request.getParameter("type").equals("searchbyroad")){
            request.setAttribute("type",request.getParameter("type"));
            String rname=request.getParameter("road");
            String distancestr=request.getParameter("distance");
            request.setAttribute("rname",rname);
            request.setAttribute("distance",distancestr);
            if(rname==null|| rname.equals("")||distancestr==null||distancestr.equals("")){
                request.setAttribute("message","查询条件不能为空");
                if(request.getSession().getAttribute("type").equals("user")) {
                    request.getRequestDispatcher("User.jsp").forward(request, response);
                }
                else if(request.getSession().getAttribute("type").equals("company")){
                    request.getRequestDispatcher("Company.jsp").forward(request, response);
                }
                else{
                    request.getRequestDispatcher("Admin.jsp").forward(request, response);
                }
            }
            else {
                float distance= Float.parseFloat(distancestr);
                int countnum=new Search().Count2(rname,distance);
                int countpage;
                if(countnum%10>0){
                    countpage=(countnum/10)+1;
                }
                else{
                    countpage=(countnum/10);
                }
                String PagenumStr = request.getParameter("pagenum");
                int Pagenum;
                if (PagenumStr == null||PagenumStr.equals("")) {
                    Pagenum = 1;
                } else {
                    Pagenum = Integer.parseInt(PagenumStr);
                }
                PageRequest pageRequest =new Search().SearchbyRoadPage(rname,distance,Pagenum);
                pageRequest.setTotalcount(countnum);
                pageRequest.setTotalpage(countpage);
                pageRequest.setPre(Pagenum==1?1:Pagenum-1);
                pageRequest.setNext(Pagenum==countpage?countpage:Pagenum+1);
                request.setAttribute("result",pageRequest);
                if (request.getSession().getAttribute("type").equals("user")) {
                    request.getRequestDispatcher("User.jsp").forward(request, response);
                } else if (request.getSession().getAttribute("type").equals("company")) {
                    request.getRequestDispatcher("Company.jsp").forward(request, response);
                } else {
                    request.getRequestDispatcher("Admin.jsp").forward(request, response);
                }
            }
        }
        //修改房源
        else if(request.getParameter("type").equals("alter")){
            request.setAttribute("type",request.getParameter("type"));
            int id= Integer.parseInt(request.getParameter("id"));
            House house=new Search().SearchHousebyID(id);
            request.setAttribute("house",house);
            request.getRequestDispatcher("AddOrAlterHouse.jsp").forward(request,response);
        }
        //删除房源
        else if(request.getParameter("type").equals("delete")){
            request.setAttribute("type",request.getParameter("type"));
            int id=Integer.parseInt(request.getParameter("id"));
            Object[] inf=new Object[1];
            inf[0]=id;
            boolean flag=new Company_Manage().Delete(inf);
            if(flag){
                request.setAttribute("message","删除成功！请刷新");
                request.getRequestDispatcher("Company.jsp").forward(request,response);
            }
            else if(!flag){
                request.setAttribute("message","删除失败！失败原因:可能该房源已经被删除");
                request.getRequestDispatcher("Company.jsp").forward(request,response);
            }
        }
        //评论
        else if(request.getParameter("type").equals("comment")){
            request.setAttribute("type",request.getParameter("type"));
            int id=Integer.parseInt(request.getParameter("id"));
            request.setAttribute("id",id);
            request.getRequestDispatcher("Comment.jsp").forward(request,response);
        }
        //查看小区
        else if(request.getParameter("type").equals("neigh")){
            String nname=request.getParameter("nname");//小区名
            request.setAttribute("nname",nname);
            List<House> houseList=new Search().Searchbynname(nname);//小区内房源
            List<Hospital> hospitalList=new Search().SearchSurroundHospital(nname);//小区附近的医院
            List<Market> marketList=new Search().SearchSurroundMarket(nname);//小区附近的超市
            List<Park> parkList=new Search().SearchSurroundPark(nname);//小区附近的公园
            List<Road> roadList=new Search().SearchSurroundRoad(nname);//小区附近的道路
            List<School> schoolList=new Search().SearchSurroundSchool(nname);//小区附近的学校
            request.setAttribute("houseList",houseList);
            request.setAttribute("hospitalList",hospitalList);
            request.setAttribute("marketList",marketList);
            request.setAttribute("parkList",parkList);
            request.setAttribute("roadList",roadList);
            request.setAttribute("schoolList",schoolList);
            String user= (String) request.getSession().getAttribute("type");
            request.setAttribute("user",user);
            request.getRequestDispatcher("Neigh.jsp").forward(request,response);
        }
        //修改小区
        else if(request.getParameter("type").equals("altern")){
            request.setAttribute("type",request.getParameter("type"));
            String nname=request.getParameter("nname");
            Neigh neigh=new Search().SearchNeighbyNname(nname);
            request.setAttribute("neigh",neigh);
            request.getRequestDispatcher("AddOrAlterNeigh.jsp").forward(request,response);
        }
        //删除小区
        else if(request.getParameter("type").equals("deleten")){
            request.setAttribute("type",request.getParameter("type"));
            String nname=request.getParameter("nname");
            Neigh neigh=new Search().SearchNeighbyNname(nname);
            Object[] inf=new Object[2];//小区主码
            inf[0]=neigh.getNname();
            inf[1]=neigh.getNtype();
            if(new Company_Manage().Delete(inf)){
                request.setAttribute("message","删除成功！请刷新");
                request.getRequestDispatcher("Company.jsp").forward(request,response);
            }
            else{
                request.setAttribute("message","删除失败！失败原因:可能该小区已经被删除");
                request.getRequestDispatcher("Company.jsp").forward(request,response);
            }
        }
        //进入后台
        else if(request.getParameter("type").equals("background")){
            List<User> userList=new Search().SearchUser();
            List<Company>  companyList=new Search().SearchCompany();
            request.setAttribute("UL",userList);
            request.setAttribute("CL",companyList);
            request.setAttribute("type",request.getParameter("type"));
            request.getRequestDispatcher("Background.jsp").forward(request,response);
        }
        //查看地图
        else if(request.getParameter("type").equals("map")){
            List<Neigh> neighList=new Search().SearchNeigh();
            request.setAttribute("neighList",neighList);
            request.getRequestDispatcher("Map.jsp").forward(request,response);
        }
    }
}
