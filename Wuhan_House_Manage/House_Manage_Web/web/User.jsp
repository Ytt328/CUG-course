<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ page isELIgnored="false" %>
<%--
  Created by IntelliJ IDEA.
  User: yangt
  Date: 2021/6/26
  Time: 18:25
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>

<!--弹窗提示信息-->
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

<html>
<head>
    <script type="text/javascript"
            src="http://api.map.baidu.com/api?v=2.0&ak=uUPEGRCNe9oGvEfhGfPOdjXHbPxSmATs">
    </script>
    <title>游客服务</title>
</head>
<body>
<div id="container" style="width:-moz-max-content">
    <div id="menu" style="height:max-content;width:auto;float:left;overflow-y: auto">
        <form method="post" action="HomepageServlet" id="queryform">
            <input type="hidden" name="pagenum" id="pagenum"><!--页数-->
            <input type="hidden" name="type" id="type" value="${type}"><!--执行的操作-->
            <div id="menu1" style="border: #cccccc; height: 200px;width: 350px;float: bottom;">
                <table border="1px">
                    <tr style="background-color: #2175BC">
                        <th colspan="2">按条件查询</th>
                    </tr>
                    <tr>
                        <td>行政区</td>
                        <td><select name="xz" id="xz" >
                            <option value="" <c:if test="${filter[0]==''}">selected</c:if>>不限</option>
                            <option value="江岸区" <c:if test="${filter[0]=='江岸区'}">selected</c:if>>江岸区</option>
                            <option value="江汉区" <c:if test="${filter[0]=='江汉区'}">selected</c:if>>江汉区</option>
                            <option value="硚口区" <c:if test="${filter[0]=='硚口区'}">selected</c:if>>硚口区</option>
                            <option value="汉阳区" <c:if test="${filter[0]=='汉阳区'}">selected</c:if>>汉阳区</option>
                            <option value="武昌区" <c:if test="${filter[0]=='武昌区'}">selected</c:if>>武昌区</option>
                            <option value="青山区" <c:if test="${filter[0]=='青山区'}">selected</c:if>>青山区</option>
                            <option value="洪山区" <c:if test="${filter[0]=='洪山区'}">selected</c:if>>洪山区</option>
                            <option value="蔡甸区" <c:if test="${filter[0]=='蔡甸区'}">selected</c:if>>蔡甸区</option>
                            <option value="江夏区" <c:if test="${filter[0]=='江夏区'}">selected</c:if>>江夏区</option>
                            <option value="黄陂区" <c:if test="${filter[0]=='黄陂区'}">selected</c:if>>黄陂区</option>
                            <option value="新洲区" <c:if test="${filter[0]=='新洲区'}">selected</c:if>>新洲区</option>
                            <option value="东西湖区" <c:if test="${filter[0]=='东西湖区'}">selected</c:if>>东西湖区</option>
                            <option value="汉南区" <c:if test="${filter[0]=='汉南区'}">selected</c:if>>汉南区</option>
                        </select></td>
                    </tr>
                    <tr>
                        <td>小区</td>
                        <td><input type="text" name="neigh" size="20" value="${filter[1]==null?"":filter[1]}"></td>
                    </tr>
                    <tr>
                        <td>价格区间(万)</td>
                        <td>
                            <input type="number" name="min_price" min="0" max="3000" value="${filter[2]==null?"":filter[2]}"> -
                            <input type="number" name="max_price" min="0" max="3000" value="${filter[3]==null?"":filter[3]}">
                        </td>
                    <tr/>
                    <tr>
                        <td>户型</td>
                        <td><select name="shi" >
                            <option value="" <c:if test="${filter[4]==''}">selected</c:if>>不限</option>
                            <option value="0" <c:if test="${filter[4]=='0'}">selected</c:if>>0</option>
                            <option value="1" <c:if test="${filter[4]=='1'}">selected</c:if>>1</option>
                            <option value="2" <c:if test="${filter[4]=='2'}">selected</c:if>>2</option>
                            <option value="3" <c:if test="${filter[4]=='3'}">selected</c:if>>3</option>
                            <option value="4" <c:if test="${filter[4]=='4'}">selected</c:if>>4</option>
                            <option value="5" <c:if test="${filter[4]=='5'}">selected</c:if>>5</option>
                            <option value="6" <c:if test="${filter[4]=='6'}">selected</c:if>>6</option>
                            <option value="7" <c:if test="${filter[4]=='7'}">selected</c:if>>7</option>
                            <option value="8" <c:if test="${filter[4]=='8'}">selected</c:if>>8</option>
                            <option value="9" <c:if test="${filter[4]=='9'}">selected</c:if>>9</option>
                            <option value="10" <c:if test="${filter[4]=='10'}">selected</c:if>>10</option>
                        </select>室
                            <select name="ting" >
                                <option value="" <c:if test="${filter[5]==''}">selected</c:if>>不限</option>
                                <option value="0" <c:if test="${filter[5]=='0'}">selected</c:if>>0</option>
                                <option value="1" <c:if test="${filter[5]=='1'}">selected</c:if>>1</option>
                                <option value="2" <c:if test="${filter[5]=='2'}">selected</c:if>>2</option>
                                <option value="3" <c:if test="${filter[5]=='3'}">selected</c:if>>3</option>
                                <option value="4" <c:if test="${filter[5]=='4'}">selected</c:if>>4</option>
                                <option value="5" <c:if test="${filter[5]=='5'}">selected</c:if>>5</option>
                                <option value="6" <c:if test="${filter[5]=='6'}">selected</c:if>>6</option>
                                <option value="7" <c:if test="${filter[5]=='7'}">selected</c:if>>7</option>
                                <option value="8" <c:if test="${filter[5]=='8'}">selected</c:if>>8</option>
                                <option value="9" <c:if test="${filter[5]=='9'}">selected</c:if>>9</option>
                                <option value="10" <c:if test="${filter[5]=='10'}">selected</c:if>>10</option>
                            </select>厅
                            <select name="wei" >
                                <option value="" <c:if test="${filter[6]==''}">selected</c:if>>不限</option>
                                <option value="0" <c:if test="${filter[6]=='0'}">selected</c:if>>0</option>
                                <option value="1" <c:if test="${filter[6]=='1'}">selected</c:if>>1</option>
                                <option value="2" <c:if test="${filter[6]=='2'}">selected</c:if>>2</option>
                                <option value="3" <c:if test="${filter[6]=='3'}">selected</c:if>>3</option>
                                <option value="4" <c:if test="${filter[6]=='4'}">selected</c:if>>4</option>
                                <option value="5" <c:if test="${filter[6]=='5'}">selected</c:if>>5</option>
                                <option value="6" <c:if test="${filter[6]=='6'}">selected</c:if>>6</option>
                                <option value="7" <c:if test="${filter[6]=='7'}">selected</c:if>>7</option>
                                <option value="8" <c:if test="${filter[6]=='8'}">selected</c:if>>8</option>
                                <option value="9" <c:if test="${filter[6]=='9'}">selected</c:if>>9</option>
                                <option value="10" <c:if test="${filter[6]=='10'}">selected</c:if>>10</option>
                            </select>卫
                        </td>
                    </tr>
                    <tr>
                        <td>面积区间(平方米)</td>
                        <td>
                            <input type="number" name="min_size" min="0" max="1000" value="${filter[7]}"> -
                            <input type="number" name="max_size" min="0" max="1000" value="${filter[8]}">
                        </td>
                    </tr>
                    <tr>
                        <th colspan="2" ><button onclick="search1()">查询</button></th>
                    </tr>
                </table>
            </div>
            <br/>
            <div id="menu2" style="border: #cccccc; height: 100px;width: 350px;float: bottom">
                <table border="1px" width="350px">
                    <tr style="background-color: #2175BC">
                        <th colspan="2">按道路查询</th>
                    </tr>
                    <tr>
                        <td>道路名</td>
                        <td><input type="text" name="road" size="20" value="${rname}"></td>
                    </tr>
                    <tr>
                        <td>查询距离范围</td>
                        <td><input type="number" name="distance" min="1" max="100000" value="${distance}"></td>
                    </tr>
                    <tr>
                        <th colspan="2"><button onclick="search2()">查询</button> </th>
                    </tr>
                </table>
            </div>
            <br/>
            <div id="map" style="border:1px solid #C0C0C0;width: 350px;height: 350px;float: bottom"></div>
        </form>
    </div>
    <div id="content" style="background-color: #EEEEEE; height: auto;width:850px;float:left;overflow-y: auto">
        <table border="1" style="font-style: normal;font-size: small">
            <tr>
                为你找到${result.totalcount}条数据:

                <a href="Login.jsp" style="float: right;color: red;margin-left: 10px">退出登录</a>
                <a href="Map.jsp" style="color: firebrick;float: right">地图</a>
            </tr>
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
                <td>操作</td>
            </tr>
            <c:forEach items="${result.rs}" var="house">
                <tr>
                    <td>${house.ID}</td>
                    <td>${house.housetype}</td>
                    <td>${house.price}</td>
                    <td>${house.avgprice}</td>
                    <td>${house.size}</td>
                    <td>${house.innersize}</td>
                    <td>${house.towards}</td>
                    <td>
                         <a href="HomepageServlet?type=neigh&nname=${house.nname}" style="color: green">${house.nname}</a>
                    </td>
                    <td>${house.hfloor}</td>
                    <td>${house.tfloor}</td>
                    <td>${house.buildyear}</td>
                    <td>${house.buildtype}</td>
                    <td>${house.comment}</td>
                    <td>${house.area}</td>
                    <td><a href="HomepageServlet?type=comment&id=${house.ID}">评论</a> </td>
                </tr>
            </c:forEach>
            <tr>
                <th colspan="15">
                    <button onclick="changepage(1)" style="color: #2175BC">首页</button>
                    <button onclick="changepage(${result.pre})" style="color: #2175BC">上一页</button>
                    <button onclick="changepage(${result.next})" style="color: #2175BC">下一页</button>
                    <button onclick="changepage(${result.totalpage})" style="color: #2175BC">尾页</button>
                    共${result.totalpage}页 当前第${result.currentpage}页
                </th>
            </tr>
        </table>
    </div>
</div>
<script>
    //条件查询
    function search1(){
        document.getElementById("type").value="searchbyfilter";
        document.getElementById("pagenum").value=1;
        document.getElementById("queryform").submit();
    }
    //按道路查询
    function search2() {
        document.getElementById("type").value="searchbyroad";
        document.getElementById("pagenum").value=1;
        document.getElementById("queryform").submit();
    }
    //切换页
    function changepage(num){
        document.getElementById("pagenum").value=num;
        document.getElementById("queryform").submit();
    }
    //地图
    function map(){
        document.getElementById("type").value="map";
        document.getElementById("queryform").submit();
    }
</script>
</body>
</html>

<script type="text/javascript">
    var map = new BMap.Map("map");
    var point = new BMap.Point(114.311831,30.598428);//武汉
    map.centerAndZoom(point, 16); //初始化地图,设置中心点坐标和地图级别。
    map.enableScrollWheelZoom(true); //启用滚轮放大缩小
    map.addControl(new BMap.NavigationControl());//向地图中添加缩放控件
    var ctrlSca = new window.BMap.ScaleControl({
        anchor: BMAP_ANCHOR_BOTTOM_LEFT
    });
    map.addControl(ctrlSca);//比例尺
    //地图、卫星、混合模式切换
    map.addControl(new BMap.MapTypeControl({mapTypes: [BMAP_NORMAL_MAP, BMAP_SATELLITE_MAP, BMAP_HYBRID_MAP]}));
</script>
