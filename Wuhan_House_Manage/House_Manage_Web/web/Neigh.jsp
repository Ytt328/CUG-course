<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ page isELIgnored="false" %>
<%--
  Created by IntelliJ IDEA.
  User: yangt
  Date: 2021/7/10
  Time: 20:19
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <script type="text/javascript"
            src="http://api.map.baidu.com/api?v=2.0&ak=uUPEGRCNe9oGvEfhGfPOdjXHbPxSmATs">
    </script>
    <title>小区详情</title>
</head>
<body>
<h1>${nname}:</h1>
<div style="width: -moz-max-content">
    <div id="menu" style="height:100%;width:20%;float:left;overflow-y: auto">
        <table border="1" style="float: top;width: 100%">
            <tr style="background-color: #2175BC">
                <th colspan="1">1000米内的医院</th>
            </tr>
            <c:forEach items="${hospitalList}" var="hospital">
                <tr>
                    <td>${hospital.hname}</td>
                </tr>
            </c:forEach>
        </table>
        <br/>
        <table border="1" style="float: top;width: 100%">
            <tr style="background-color: #2175BC">
                <th colspan="1">1000米内的超市</th>
            </tr>
            <c:forEach items="${marketList}" var="market">
                <tr>
                    <td>${market.mname}</td>
                </tr>
            </c:forEach>
        </table>
        <br/>
        <table border="1" style="float: top;width: 100%">
            <tr style="background-color: #2175BC">
                <th colspan="1">1000米内的公园</th>
            </tr>
            <c:forEach items="${parkList}" var="park">
                <tr>
                    <td>${park.pname}</td>
                </tr>
            </c:forEach>
        </table>
        <br/>
        <table border="1" style="float: top;width: 100%">
            <tr style="background-color: #2175BC">
                <th colspan="1">1000米内的道路</th>
            </tr>
            <c:forEach items="${roadList}" var="road">
                <tr>
                    <td>${road.rname}</td>
                </tr>
            </c:forEach>
        </table>
        <br/>
        <table border="1" style="float: top;width: 100%">
            <tr style="background-color: #2175BC">
                <th colspan="1">1000米内的学校</th>
            </tr>
            <c:forEach items="${schoolList}" var="school">
                <tr>
                    <td>${school.sname}</td>
                </tr>
            </c:forEach>
        </table>
    </div>
    <div id="content" style="background-color: #EEEEEE; height: 100%;width:80%;float:left;overflow-y: auto">
        <table border="1" style="width: auto;height: auto;font-size: small">
    <!--表头-->
    <tr style="background-color: #DBEBD3">
        <td>ID</td>
        <td>户型</td>
        <td>总价(万)</td>
        <td>均价(元/平方米)</td>
        <td>建筑面积(平方米)</td>
        <td>套内面积(平方米)</td>
        <td>朝向</td>
        <td>小区</td>
        <td>楼层</td>
        <td>总楼层</td>
        <td>建筑年份</td>
        <td>建筑类型</td>
        <td>评论</td>
        <td>行政区</td>
        <c:if test="${user=='user'||user=='company'}">
            <td>操作</td>
        </c:if>
    </tr>
    <!--表内容-->
    <c:forEach items="${houseList}" var="house">
        <tr>
            <td>${house.ID}</td>
            <td>${house.housetype}</td>
            <td>${house.price}</td>
            <td>${house.avgprice}</td>
            <td>${house.size}</td>
            <td>${house.innersize}</td>
            <td>${house.towards}</td>
            <td>${house.nname}</td>
            <td>${house.hfloor}</td>
            <td>${house.tfloor}</td>
            <td>${house.buildyear}</td>
            <td>${house.buildtype}</td>
            <td>${house.comment}</td>
            <td>${house.area}</td>
            <c:if test="${user=='user'}">
                <td><a href="HomepageServlet?type=comment&id=${house.ID}">评论</a> </td>
            </c:if>
            <c:if test="${user=='company'}">
                <td>
                    <a href="HomepageServlet?type=alter&id=${house.ID}" style="color: #2175BC;margin-right: 10px">修改</a>
                    <a href="HomepageServlet?type=delete&id=${house.ID}" style="color: red">删除</a>
                </td>
            </c:if>
        </tr>
    </c:forEach>
</table>
    </div>
</div>
</body>
</html>



