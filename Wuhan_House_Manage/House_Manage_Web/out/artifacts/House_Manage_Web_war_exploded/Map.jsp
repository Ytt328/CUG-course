<%--
  Created by IntelliJ IDEA.
  User: yangt
  Date: 2021/7/12
  Time: 20:22
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <script type="text/javascript"
            src="http://api.map.baidu.com/api?v=2.0&ak=uUPEGRCNe9oGvEfhGfPOdjXHbPxSmATs">
    </script>
    <title>地图</title>
</head>
<body>
<div id="container" style="width: 100%;height: 100%"></div>
<div id="r-result" style="display: inline-block;width:12%;position: absolute;top:10px;left:100px;">
    <input type="text" id="suggestId" size="20" value="百度" placeholder="请输入.." style="width:150px;height:20px;"/>
    <input type="button" οnclick="getBoundary()" value="获取轮廓线">
</div>
<div id="searchResultPanel" style="border:1px solid #C0C0C0;width:150px;height:auto; display:none;"></div>
</body>

</html>
<script type="text/javascript">
    var map = new BMap.Map("container");
    var point = new BMap.Point(114.311831,30.598428);//武汉
    map.setCurrentCity("武汉");
    map.centerAndZoom(point, 15); //初始化地图,设置中心点坐标和地图级别。
    map.enableScrollWheelZoom(true); //启用滚轮放大缩小
    map.addControl(new BMap.NavigationControl());//向地图中添加缩放控件
    var ctrlSca = new window.BMap.ScaleControl({
        anchor: BMAP_ANCHOR_BOTTOM_LEFT
    });
    map.addControl(ctrlSca);//比例尺
    //地图、卫星、混合模式切换
    map.addControl(new BMap.MapTypeControl({mapTypes: [BMAP_NORMAL_MAP, BMAP_SATELLITE_MAP, BMAP_HYBRID_MAP]}));

    var geoc = new BMap.Geocoder();
    map.addEventListener("click", function (e) {
        var pt = e.point;
        geoc.getLocation(pt, function(rs){
            console.info(rs)
            var addComp = rs.addressComponents;
            console.info("经纬度:(" + e.point.lng + "," + e.point.lat+");  "+addComp.province + ", " + addComp.city + ", " + addComp.district + ", " + addComp.street + ", " + addComp.streetNumber);
        });
    });

    //根据地名定位（1）
    function theLocation() {
        var city = document.getElementById("cityName").value;
        if (city != "") {
            map.centerAndZoom(city, 11);      // 用城市名设置地图中心点
        }
    }

    //根据输入查找定位（2）
    var ac = new BMap.Autocomplete(    //建立一个自动完成的对象
        {
            "input": "suggestId"
            , "location": map
        });

    ac.addEventListener("onhighlight", function (e) {  //鼠标放在下拉列表上的事件
        var str = "";
        var _value = e.fromitem.value;
        var value = "";
        if (e.fromitem.index > -1) {
            value = _value.province + _value.city + _value.district + _value.street + _value.business;
        }
        str = "FromItem<br />index = " + e.fromitem.index + "<br />value = " + value;

        value = "";
        if (e.toitem.index > -1) {
            _value = e.toitem.value;
            value = _value.province + _value.city + _value.district + _value.street + _value.business;
        }
        str += "<br />ToItem<br />index = " + e.toitem.index + "<br />value = " + value;
        document.getElementById("searchResultPanel").innerHTML = str;
    });

    var myValue;
    ac.addEventListener("onconfirm", function (e) {    //鼠标点击下拉列表后的事件
        var _value = e.item.value;
        myValue = _value.province + _value.city + _value.district + _value.street + _value.business;
        document.getElementById("searchResultPanel").innerHTML = "onconfirm<br />index = " + e.item.index + "<br />myValue = " + myValue;

        setPlace();
    });

    function setPlace() {
        map.clearOverlays();    //清除地图上所有覆盖物
        function myFun() {
            var pp = local.getResults().getPoi(0).point;    //获取第一个智能搜索的结果
            map.centerAndZoom(pp, 18);
            map.addOverlay(new BMap.Marker(pp));    //添加标注
        }

        var local = new BMap.LocalSearch(map, { //智能搜索
            onSearchComplete: myFun
        });
        local.search(myValue);
    }


    //修改图标
    var pt = new BMap.Point(116.417, 39.909);
    var img = "pic.png";
    var myIcon = new BMap.Icon(img, new BMap.Size(300, 157));
    var marker2 = new BMap.Marker(pt, {icon: myIcon});  // 创建标注
    map.addOverlay(marker2);              // 将标注添加到地图中
    var record = [];
    marker2.addEventListener('click',function () {
        //点击标注时切换标注，同时记录标注内容；再次点击标注时换回原来标注，并将该标注内容从记录中去除
        if(img == "pic.png"){
            record.push(this);
        }else{
            for(var i in record){
                if(record[i] == this){
                    record.splice(i,1)
                }
            }
        }
        img = img=="pic.png"?"video.png":"pic.png";
        this.setIcon(new BMap.Icon(img,new BMap.Size(300, 157)));

        console.info(record)
    })


</script>


