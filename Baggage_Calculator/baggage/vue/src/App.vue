<template>
  <div>
    <el-container>
      <el-header style="width: 100%;text-align:center">国航行李托运计算器</el-header>
      <el-container>
        <el-container style="width: 25%;height: 320px;margin-left: 10%;border: 1px solid gray">
          <table width="100%" height="100%">
            <tr>
              <th colspan="2" >机票详情</th>
            </tr>
            <tr>
              <td>航线类型</td>
              <td>
                <el-cascader id="routetype" v-model="ticket_baggage.ticket.routetype" :options="rtypeoptions"  @change="routechange" placeholder="请选择"></el-cascader>
              </td>
            </tr>
            <tr>
              <td>机舱类型</td>
              <td>
                <el-select id="cabintype" v-model="ticket_baggage.ticket.cabintype"  placeholder="请选择">
                  <el-option-group v-for="group in ctypeoptions" :key="group.label" :label="group.label">
                    <el-option v-for="item in group.ctypeoptions" :key="item.value" :label="item.label" :value="item.value"></el-option>
                  </el-option-group>
                </el-select>
              </td>
            </tr>
            <tr>
              <td>乘客类型</td>
              <td>
                <el-select id="passenger" v-model="ticket_baggage.ticket.passenger" placeholder="请选择">
                  <el-option v-for="item in ptypeoptions" :key="item.value" :label="item.label" :value="item.value"></el-option>
                </el-select>
              </td>
            </tr>
            <tr>
              <td>会员类型</td>
              <td>
                <el-select id="vip" v-model="ticket_baggage.ticket.vip" placeholder="请选择(国际航线不用选此项)">
                  <el-option v-for="item in vtypeoptions" :key="item.value" :label="item.label" :value="item.value"></el-option>
                </el-select>
              </td>
            </tr>
            <tr>
              <td>机票价格</td>
              <td>
                <el-input id="price" type="number" min="0" v-model="ticket_baggage.ticket.price" placeholder="单位：元"></el-input>
              </td>
            </tr>
            <tr>
              <th colspan="2">
                <el-checkbox v-model="ticket_baggage.ticket.isdisable" label="是否残疾、伤、病"></el-checkbox>
                <el-button style="float: right" @click="calculate">计算</el-button>
              </th>
            </tr>
          </table>
        </el-container>
        <el-container style="width: 55%;height: 320px;margin-right: 10%;border: 1px solid gray">
          <el-table :data="ticket_baggage.tableData" width="100%" height="100%">
            <el-table-column prop="type" label="行李类型" width="150">
              <template #default="scope">
                <el-select v-model="scope.row.type" placeholder="请选择">
                  <el-option
                      v-for="item in baggagetype"
                      :label="item.label"
                      :value="item.value">
                  </el-option>
                </el-select>
              </template>
            </el-table-column>
            <el-table-column prop="length"  label="长(厘米)">
              <template #default="scope">
                <el-input type="number" min="0" max="200" v-model="scope.row.length"></el-input>
              </template>
            </el-table-column>
            <el-table-column prop="width"  label="宽(厘米)">
              <template #default="scope">
                <el-input type="number" min="0" max="200" v-model="scope.row.width"></el-input>
              </template>
            </el-table-column>
            <el-table-column prop="height"  label="高(厘米)">
              <template #default="scope">
                <el-input type="number" min="0" max="200" v-model="scope.row.height"></el-input>
              </template>
            </el-table-column>
            <el-table-column prop="weight" label="质量(千克)">
              <template #default="scope">
                <el-input type="number" min="2" max="32" v-model="scope.row.weight"></el-input>
              </template>
            </el-table-column>
            <el-table-column prop="right" label="操作" :render-header="renderHeader">
              <template #default="scope">
                <el-button
                    @click.prevent="deleteRow(scope.$index, ticket_baggage.tableData)"
                    type="text"
                    size="small"
                    icon="el-icon-delete">删除
                </el-button>
              </template>
            </el-table-column>
          </el-table>
        </el-container>
      </el-container>

      <el-footer style="width: 80%;margin-left: 10%">
        <div style="background-color: lightgray">提示1:</div>
        <div>
          区域一：<span>美洲（除美国/加拿大外）/加勒比海地区 => 欧洲/非洲/中东/亚洲/西南太平洋</span><br>
          区域二：<span>欧洲/中东 => 非洲/亚洲/西南太平洋</span> |
          <span>日本 => 西南太平洋</span> |
          <span>日本/西南太平洋 => 亚洲（不含日本及西南太平洋）/非洲</span><br>
          区域三：<span>加拿大 => 美洲（除美国/加拿大外）/加勒比海地区/欧洲/非洲/中东/亚洲/西南太平洋</span><br>
          区域四：<span>美国（含夏威夷） => 美洲（除美国外）/加勒比海地区/欧洲/非洲/中东/亚洲/西南太平洋</span><br>
          区域五：<span>非洲 => 亚洲（除日本外)</span> |
          <span>欧洲 => 中东</span> |
          <span>亚洲（除日本)内航线</span> |
          <span>美洲（除美国/加拿大）及加勒比海地区内航线</span> |
          <span>上述未列明的航线</span>
        </div>
        <div style="background-color: lightgray">提示2:</div>
        <div>
          国际航线经济舱(区域A)：<span>夏威夷与美洲、加勒比地区、欧洲、非洲、中东、亚洲、西南太平洋之间的航线</span>|
          <span>欧洲、非洲与亚洲 (除日本、巴基斯坦、新加坡、哈萨克斯坦外）之间的航线</span>|
          <span>亚洲内 (除日本、巴基斯坦、新加坡、哈萨克斯坦外）之间的航线</span>|
          <span>其他未列明的航线</span>
          <span>(注：亚洲含中国大陆及港澳台地区，不含中东、西南太平洋。)</span><br>
          国际航线经济舱(区域B)：<span>美洲、加勒比地区（除夏威夷）与欧洲、非洲、中东、亚洲、西南太平洋之间的航线</span>|
          <span>中东与欧洲、非洲、亚洲、西南太平洋之间航线</span>|
          <span>日本、巴基斯坦、新加坡、哈萨克斯坦、西南太平洋与 欧洲、非洲、亚洲 (除日本、巴基斯坦、新加坡、哈萨克斯坦外）之间的航线。</span>
        </div>
        <div style="background-color: lightgray">提示3:</div>
        <div>
          残疾、伤、病旅客辅助设备:
          电动轮椅/电动代步工具/手动轮椅;
          机械假肢及专用小型气瓶；
          心脏起搏器或其他植入人体的装置；
          便携式氧气浓缩机（POC）；
          持续正压呼吸机（CPAP）；
          其它内含锂电池的辅助设备等。<br>
          运动器械器具类1：高尔夫球包、保龄球、滑翔伞/降落伞、滑雪/滑水用具（不
          包括雪橇/水撬）、轮滑/滑板用具、潜水用具、射箭用具、
          曲棍球用具、冰球用具、网球用具、登山用具、自行车<br>
          运动器械器具类2：皮划艇/独木舟、悬挂式滑翔运动用具、雪橇/水撬。
          冲浪板、风帆冲浪用具、橡皮艇或船<br>
          运动器械器具类3：撑杆、标枪、单独包装的划船用具或浆、骑马用具<br>
        </div>
      </el-footer>
    </el-container>
  </div>
</template>

<style>

</style>

<script>
import { defineComponent, ref } from 'vue'
import request from "./utils/request";
import {ElMessage, ElMessageBox} from "element-plus";
export default {
  data() {
    return {
      ticket_baggage:{
        ticket:{
          routetype:"",
          cabintype:"",
          passenger:"",
          vip:"",
          price:"",
          isdisable:false,
        },//机票数据
        tableData: [],//行李数据
      },
      /*ticket:{
        routetype:"",
        cabintype:"",
        passenger:"",
        vip:"",
        price:"",
        isdisable:false,
      },//机票数据
      tableData: [],//行李数据*/
      rtypeoptions: [
        {value: '国内航线', label: '国内航线',},
        {value: '国际航线', label: '国际航线', children: [
            {value: '区域一', label: '区域一',},
            {value: '区域二', label: '区域二',},
            {value: '区域三', label: '区域三',},
            {value: '区域四', label: '区域四',},
            {value: '区域五', label: '区域五',},
          ],
        },
      ],//航线类型
      ctypeoptions:[
        {label:"国内航线", ctypeoptions:[
            {value:"头等舱", label:"头等舱",},
            {value: "公务舱",label: "公务舱"},
            {value: "经济舱",label: "经济舱"},
          ]
        },
        {label: "国际航线",ctypeoptions: [
            { value: "头等、公务舱",label: "头等、公务舱"},
            { value: "悦享经济舱、超级经济舱",label: "悦享经济舱、超级经济舱"},
            { value: "经济舱（区域A）",label: "经济舱（区域A）"},
            { value: "经济舱（区域B）",label: "经济舱（区域B）"},
          ]
        }
      ],//机舱类型
      ptypeoptions:[
        {value: "成人票",label: "成人票",},
        {value: "儿童票",label: "儿童票",},
        {value: "婴儿票",label: "婴儿票",},
      ],//乘客类型
      vtypeoptions:[
        {value: "无",label: "无"},
        {value: "凤凰知音终身白金卡、白金卡",label: "凤凰知音终身白金卡、白金卡"},
        {value: "凤凰知音金卡、银卡",label: "凤凰知音金卡、银卡"},
        {value: "星空联盟金卡",label: "星空联盟金卡"},
      ],//vip类型
      baggagetype:[
        {value:"普通行李", label:"普通行李",},
        {value:"残疾、伤、病旅客辅助设备", label:"残疾、伤、病旅客辅助设备",},
        {value:"婴儿床/婴儿车/婴儿摇篮/婴儿汽车安全座椅", label:"婴儿床/婴儿车/婴儿摇篮/婴儿汽车安全座椅",},
        {value:"导盲犬/助听犬/精神抚慰犬", label:"导盲犬/助听犬/精神抚慰犬",},
        {value: "骨灰",label: "骨灰"},
        {value:"运动器械器材类1", label:"运动器械器材类1",},
        {value:"运动器械器材类2", label:"运动器械器材类2",},
        {value:"运动器械器材类3", label:"运动器械器材类3",},
        {value: "睡袋/背包/野营用具/渔具/乐器",label: "睡袋/背包/野营用具/渔具/乐器"},
        {value:"小型电器或仪器、媒体设备",label: "小型电器或仪器、媒体设备 "},
        {value:"可作为行李运输的枪支", label:"可作为行李运输的枪支",},
        {value:"可作为行李运输的子弹", label:"可作为行李运输的子弹",},
        {value:"小动物(仅限家庭驯养的猫、狗)", label:"小动物(仅限家庭驯养的猫、狗)",},
      ],//行李类型
    }
  },
  methods: {
    renderHeader() {
      return (
          <div>
            <el-button
                type="primary"
                size="mini"
                onClick={() => this.addRow()}>添加行李
            </el-button>
          </div>
      )
    },//返回一个添加行李的按钮
    addRow() {//添加行李
      var newval = {
        type: "",
        length: "",
        width: "",
        height: "",
        weight: "",
      };
      this.ticket_baggage.tableData.push(newval);
    },
    deleteRow(index, rows) {//删除行李
      rows.splice(index, 1)
    },
    /*routechange() {
      if(this.ticket_baggage.ticket.routetype.length===2){
        this.ctypeoptions=[
          {label: "国际航线",ctypeoptions: [
              { value: "头等、公务舱",label: "头等、公务舱"},
              { value: "悦享经济舱、超级经济舱",label: "悦享经济舱、超级经济舱"},
              { value: "经济舱（区域A）",label: "经济舱（区域A）"},
              { value: "经济舱（区域B）",label: "经济舱（区域B）"},
            ]
          }
        ]

      }
      if(this.ticket_baggage.ticket.routetype.length===1){
        this.ctypeoptions=[
          {label:"国内航线", ctypeoptions:[
              {value:"头等舱", label:"头等舱",},
              {value: "公务舱",label: "公务舱"},
              {value: "经济舱",label: "经济舱"},
            ]
          }
        ]
      }
    },*/
    calculate() {//传输数据到后端计算
      if (this.ticket_baggage.ticket.cabintype === "" || this.ticket_baggage.ticket.price === "" || this.ticket_baggage.ticket.passenger === "") {
        this.$message({type: "error", message: "存在未填项！"})
      } else if (this.ticket_baggage.ticket.routetype.length === 1 && this.ticket_baggage.ticket.vip === "") {
        this.$message({type: "error", message: "存在未填项！"})
      } else if (this.ticket_baggage.ticket.routetype.length === 1 && this.ticket_baggage.ticket.cabintype !== "头等舱" && this.ticket_baggage.ticket.cabintype !== "公务舱" && this.ticket_baggage.ticket.cabintype !== "经济舱") {
        this.$message({type: "error", message: "航线类型与机舱类型不匹配！"})
      } else if (this.ticket_baggage.ticket.routetype.length === 2 && (this.ticket_baggage.ticket.cabintype === "头等舱" || this.ticket_baggage.ticket.cabintype === "公务舱" || this.ticket_baggage.ticket.cabintype === "经济舱")) {
        this.$message({type: "error", message: "航线类型与机舱类型不匹配！"})
      } else if (this.ticket_baggage.tableData.length === 0) {
        this.$message({type: "error", message: "请添加行李信息！"})
      }
      else {
        let msg;
        this.ticket_baggage.tableData.forEach(function (baggage){
          if(baggage.type===""||baggage.height===""||baggage.width===""||baggage.length===""||baggage.weight===""){
            msg="error";
          }
        })
        if(msg==="error"){
          this.$message({type: "error", message: "请完善行李信息！"})
        }
        else{
            request.post("/api/calculate", this.ticket_baggage).then(res => {
              console.log(res);
              if (res.msg === null) {
                ElMessageBox.alert("总价为:" + res.price + "元", '计算结果', {
                  confirmButtonText: '确定',
                })
              } else {
                this.$message({type: "error", message: res.msg})
              }
            })
        }
      }
      /*else {
        request.post("/api/calculate", this.ticket_baggage).then(res => {
          console.log(res);
          if(res.msg===null) {
            this.$message({type: "success", message: "总价为:" + res.price + "元"})
          }
          else{
            this.$message({type: "error", message: res.msg})
          }
        })
      }*/
    },
  }
}
</script>
