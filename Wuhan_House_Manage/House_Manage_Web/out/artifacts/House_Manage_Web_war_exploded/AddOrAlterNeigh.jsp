<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ page isELIgnored="false" %>
<%--
  Created by IntelliJ IDEA.
  User: yangt
  Date: 2021/6/25
  Time: 21:06
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>${neigh.nname==null?"添加":"修改"}小区</title>
    <link rel="stylesheet" href="css/Table.css">
</head>

<%
    String message = (String) request.getAttribute("message");
    request.getSession().setAttribute("message",null);
    if((message!=null)&&!message.equals("")){
%>
<script type="text/javascript">
    alert("<%=message%>");
</script>
<%
    }
%>
<body>
<form action="AddOrAlterNeighServlet?nname_before=${neigh.nname}" method="post">
    <table id="addneigh">
        <tr>
            <th colspan="2">${neigh.nname==null?"添加":"修改"}小区</th>
        </tr>
        <tr>
            <td>经度<label style="color: red">*</label></td>
            <td><input type="text" name="longitude" value="${neigh.longitude}"></td>
        </tr>
        <tr>
            <td>纬度<label style="color: red">*</label></td>
            <td><input type="text" name="altitude" value="${neigh.altitude}"></td>
        </tr>
        <tr>
            <td>行政区<label style="color: red">*</label></td>
            <td><input type="text" name="xzname" value="${neigh.area}"></td>
        </tr>
        <tr>
            <td>小区名<label style="color: red">*</label></td>
            <td><input type="text" name="nname" value="${neigh.nname}"></td>
        </tr>
        <tr>
            <td>销售状态<label style="color: red">*</label></td>
            <td><select name="sales" >
                <option value="在售" <c:if test="${neigh.sales=='在售'}"> selected="selected" </c:if> >在售</option>
                <option value="售罄" <c:if test="${neigh.sales=='售罄'}"> selected="selected" </c:if> >售罄</option>
            </select></td>
        </tr>
        <tr>
            <td>小区类型<label style="color: red">*</label></td>
            <td><select name="ntype" >
                <option value="住宅" <c:if test="${neigh.ntype=='住宅'}"> selected="selected" </c:if>>住宅</option>
                <option value="别墅" <c:if test="${neigh.ntype=='别墅'}"> selected="selected" </c:if>>别墅</option>
            </select></td>
        </tr>
        <tr>
            <td>价格</td>
            <td><input type="text" name="price" value="${neigh.price}"></td>
        </tr>
        <tr>
            <td>容积率</td>
            <td><input type="text" name="plot" value="${neigh.plot}"></td>
        </tr>
        <tr>
            <td>绿化率</td>
            <td><input type="text" name="green" value="${neigh.green}"></td>
        </tr>
        <tr>
            <td colspan="2" id="buttons">
                <input type="submit" value="保存">
                <input type="reset" value="取消" onclick="history.back(-1)">
            </td>
        </tr>
    </table>
</form>

</body>
</html>
