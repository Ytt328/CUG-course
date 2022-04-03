<%--
  Created by IntelliJ IDEA.
  User: yangt
  Date: 2021/6/25
  Time: 20:09
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>${house.ID==null?"添加":"修改"}房源</title>
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
     <form action="AddOrAlterHouseServlet?id=${house.ID}" method="post">
     <table id="addhouse">
         <tr>
             <th colspan="2">${house.ID==null?"添加":"修改"}房源</th>
         </tr>
         <tr>
             <td>房型(*室*厅*卫)<label style="color: red">*</label></td>
             <td><input type="text" name="housetype" value="${house.housetype}"></td>
         </tr>
         <tr>
             <td>总价(万)<label style="color: red">*</label></td>
             <td><input type="text" name="price" value="${house.price}"></td>
         </tr>
         <tr>
             <td>均价(元/平方米)<label style="color: red">*</label></td>
             <td><input type="text" name="avgprice" value="${house.avgprice}"></td>
         </tr>
         <tr>
             <td>建筑面积(平方米)<label style="color: red">*</label></td>
             <td><input type="text" name="size" value="${house.size}"></td>
         </tr>
         <tr>
             <td>套内面积(平方米)</td>
             <td><input type="text" name="innersize" value="${house.innersize}"></td>
         </tr>
         <tr>
             <td>朝向</td>
             <td><input type="text" name="towards" value="${house.towards}"></td>
         </tr>
         <tr>
             <td>小区名<label style="color: red">*</label></td>
             <td><input type="text" name="nname" value="${house.nname}"></td>
         </tr>
         <tr>
             <td>楼层</td>
             <td><input type="text" name="hfloor" value="${house.hfloor}"></td>
         </tr>
         <tr>
             <td>总楼层</td>
             <td><input type="number" name="tfloor" min="1"  value="${house.tfloor}"></td>
         </tr>
         <tr>
             <td>建筑年份<label style="color: red">*</label></td>
             <td><input type="text" name="buildyear" value="${house.buildyear}"></td>
         </tr>
         <tr>
             <td>建筑类型</td>
             <td><input type="text" name="buildtype" value="${house.buildtype}"></td>
         </tr>
         <tr>
             <td colspan="2" id="buttons">
                 <input type="submit" value="保存">
                 <input type="reset" value="取消"  onclick="history.back(-1)">
             </td>
         </tr>
     </table>
     </form>
</body>
</html>
