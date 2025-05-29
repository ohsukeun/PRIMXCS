// 통합대시보드  dashboardSePopup.js
$(document).ready(function() {
    drawGrid();
    initDashboard();
});


// 대시보드 배경그리드
var drawGrid = function() {
    const scale = window.devicePixelRatio;
    const canvas = document.querySelector("canvas");

    // 가로크기
    let screenWidth = Number($('#screenWidth').val());
    if (screenWidth > parseInt(canvas.style.width)) {
        $('#basePanel').css('width', screenWidth + 10);
        $('#canvasPanel').css('width', screenWidth + 10);
    } else {
        $('#basePanel').css('width', 1920);
        $('#canvasPanel').css('width', 1920);
    }
    // 세로크기
    let screenHeight = Number($('#screenHeight').val());
    if (screenHeight > parseInt(canvas.style.height)) {
        $('#basePanel').css('height', screenHeight + 10);
        $('#canvasPanel').css('height', screenHeight + 10);
    } else {
        $('#basePanel').css('height', 1080);
        $('#canvasPanel').css('height', 1080);
    }
    
    // 캔버스 가로 크기
    const canvasWidth = screenWidth > parseInt(canvas.style.width) ? screenWidth + 10 : parseInt(canvas.style.width);
    // 캔버스 세로 크기
    const canvasHeight = screenHeight > parseInt(canvas.style.height) ? screenHeight + 10 : parseInt(canvas.style.height);
    // 캔버스 타입(2d)
    const ctx = canvas.getContext("2d");
    ctx.scale(scale, scale);

    canvas.width = canvasWidth * scale;
    canvas.height = canvasHeight * scale;

    // 가로선
    const gridNumber = parseInt(canvasWidth / 10);
    // 세로선
    const gridNumber2 = parseInt(canvasHeight / 10);

    ctx.beginPath();
    // 가로선
    for (let idx = 0; idx <= gridNumber2; idx++) {
        ctx.moveTo(0, idx * (canvasWidth / gridNumber));
        ctx.lineTo(canvasWidth, idx * (canvasWidth / gridNumber));
    }

    // 세로선
    for (let idx = 0; idx <= gridNumber; idx++) {
        ctx.moveTo(idx * (canvasHeight / gridNumber2), 0);
        ctx.lineTo(idx * (canvasHeight / gridNumber2), canvasHeight);

    }
    ctx.strokeStyle = "#ebebeb";  // "#f7073b";
    ctx.stroke();
};

$(document).mousemove(function(event) {
    $("#xPosValue").text(event.clientX);
    $("#yPosValue").text(event.clientY);
});

// 대시보드 관리
var dashboardMst = {
    db_DASHBOARDSEQNO: 0,  /* 대시보드 ID   */
    db_DASHBOARDTYPE: "", /* 대시보드 유형 */
    db_SCREENWIDTH: 0,  /* 화면 넓이 */
    db_SCREENHEIGHT: 0,  /* 화면 높이 */
    db_REFRESHINTERVAL: 0,  /* 새로고침간격 */
    layoutInfoList: []
}

// 템플릿 관리
var chartInfo = {
    ci_CHARTID:"",          /* 차트ID   */
    ci_CHARTTYPE: "",       /* 차트유형  */
    ci_CHARTSUBTYPE: "",    /* 차트하위유형 */
    ci_CHARTOPTIONS: "",    /* 차트데이터   */
    ci_CHARTIMAGE: "",      /* 차트아이콘이미지 */
    ci_CHARTCOMMENT: "",    /* 비고    */
    ci_REGDATE: ""          /* 등록일자 */
}

var deleteDashboardCnt = 0;
var baseClear = false; // 화면초기화 여부

// Apexchart 보관
var chartArray = [];

 // 템플릿선택 팝업 리스트
var templateResult = [];

// 공통메세지
$( "#resultMessage" ).hide();

// 날자 포맷
var dateFormat = function(date) {
    var month = date.getMonth() + 1;
    var day = date.getDate();
    var hour = date.getHours();
    var minute = date.getMinutes();
    var second = date.getSeconds();

    var twoDigit = digNum => digNum >= 10 ? digNum.toString() : '0' + digNum;
    month = twoDigit(month);
    day = twoDigit(day);
    hour = twoDigit(hour);
    minute = twoDigit(minute);
    second = twoDigit(second);
    return date.getFullYear().toString() + month + day + hour + minute + second;
}

var dCount = 0;  // 일련번호
/*
var getSeqNo = function() {
    // 현재 날짜시간
    var retVal = dateFormat(new Date()) + dCount++;
    return retVal;
}
*/

// 레이아웃 작성
var baseStyle = {
    left: "1px",
    top: "1px",
    width: "0px",
    height: "0px",
    widthRate: 0,
    heightRate: 0,
    layoutType: "",   // vertical 세로  horizontal 가로
    layoutObject: null,
};

var styleProp = {
    position: "absolute",
    left: "1px",
    top: "1px",
    width: "1px",
    height: "1px",
};

// layout 선택 결과
var layoutTypeRetVal = {
    layoutType: "",
    layOUtValue: 0
}

var selectId = "";  // 셀 병합대상 layout Id
var divCnt = 0;
var templateObj = null;  // 템플릿 관리 차트
/**
 * 템플릿 초기화
 */
var initTemplateMng = function() {
    chartInfo.ci_CHARTTYPE    = "";
    chartInfo.ci_CHARTID      = "";
    chartInfo.ci_CHARTIMAGE   = "";
    chartInfo.ci_CHARTCOMMENT = "";
    chartInfo.ci_CHARTOPTIONS = "";
    chartInfo.ci_CHARTSUBTYPE = "";

    $("#chartTypeSelect option:eq(0)").prop("selected", true);
    $("#chartId").val("");
    $("#chartImage").val("");
    $("#chartComment").val("");
    $("#chartOptions").val("");
    $("#chartPreview").empty();
            
    $("#chartTypeSelect").attr("disabled", false);
    $("#chartId").attr("disabled", false);

    if ( templateObj ) {
        templateObj.destroy();
        templateObj = null;
    }
    
}

/**
 * 공통 메세지 팝업
 */
var messagePop = function(msg) {
    $("#msgContent").text(msg);
    $( "#resultMessage" ).show( "slide", {}, 500, setTimeout( () => {
        $( "#resultMessage:visible" ).removeAttr( "style" ).fadeOut();
    }, 1000 ));
}

var drawChart = function(chartObj, optionsData) {
    var chart = new ApexCharts(chartObj, optionsData);
    // chart.render();
    return chart;
}

var drawListChart = function(chartObj, optionsData) {
    var tableObj = new DataList(chartObj, optionsData);
    return tableObj;
}


// 삭제된 차트 정리
var chartManager = function(chartId) {

    chartArray.forEach((element, idx) => {
/*
        console.dir(element);
        console.log(element.el.id);
*/
        if (element.el.id == chartId) {
//            console.log(element.el.id + " 차트 destroy()");
            element.destroy();
            chartArray[idx] = null;
        }
    });

//    console.log("전 ========================");
//    console.dir(chartArray);
    if (chartArray.includes(null)) {
        var newChartArray = chartArray.filter(
            (element) => element != null
        );
        chartArray = newChartArray;
    }
//    console.dir(chartArray);
//    console.log("후 ========================");
}

// 차트 레이아웃 생성
var createChartLayout = function(chartPos) {
//    console.log("createChartLayout ", chartPos);
    var $div = $("<div></div>");
    $div.css(chartPos);
    $div.addClass("div-pan-layout");
    $div.attr("id", "div-cell-" + divCnt);
    $div.append($("<div id='div-cell-" + divCnt + "-chart' class='div-chart-layout' ></div>"));

    var btnClone = $("#popModal_ex1").clone(true);
    $div.append(btnClone);
    $("#basePanel").append($div);
    divCnt++;
    return "div-cell-" + (divCnt - 1);
}


// 차트 레이아웃 생성
var loadChartLayout = function(chartPos, layoutId) {
//    console.log("createChartLayout ", chartPos);
    var $div = $("<div></div>");
    $div.css(chartPos);
    $div.addClass("div-pan-layout");
    $div.attr("id", layoutId);
    $div.append($("<div id='" + layoutId + "-chart' class='div-chart-layout' ></div>"));

    var btnClone = $("#popModal_ex1").clone(true);
    $div.append(btnClone);
    $("#basePanel").append($div);
}


// 

var removeLayout = function(obj) {
//    console.log("removeLayout ----------------");
//    console.log($(obj).attr("id") + " parent() ======");
//    console.log(obj + "-chart");
//    console.log("object" + $(obj + "-chart").attr("id"));

    chartManager($(obj + "-chart").attr("id"));
    $(obj).remove();
}

var initDashboard = function() {
    $("#dashboardSeqNo").val(0);
    $("#dashboardType").val("");
    $("#refreshInterval").val(0);
    $("#screenWidth").val("");
    $("#screenHeight").val("");
}

// 초기화
var removeChart = function() {
//    console.log("removeChart ----------------");
    var idx = chartArray.length;
    for (var i = idx; i > 0; i--) {
//        console.dir(chartArray[i - 1]);
//        console.log(" 차트 destroy()");
        chartArray.pop().destroy();
    }

    for (var i = 0; i < $("#basePanel > .div-pan-layout").length; i++) {
        var curId = $($("#basePanel > .div-pan-layout")[i]).attr("id");
        $("#" + curId + "-chart").empty();
    }
}

// 가로형 horizontalType layout
var horizontalTypeLayout = function(layoutId) {
    var wRate = baseStyle.widthRate;
//    console.log("가로형 verticalType ============layoutId " + layoutId);
//    console.log($(layoutId).attr("id") + " = popModal_ex1");

    var baseLeft = $(layoutId).position().left;
    var baseTop = $(layoutId).position().top;
    var baseWidth = parseInt($(layoutId).css("width"));
    var baseHeight = parseInt($(layoutId).css("height"));

    var tgtWidth = Math.round(baseWidth * wRate);

    removeLayout(layoutId);

    // createLayout
    styleProp.left = baseLeft + "px";
    styleProp.top = baseTop + "px";
    styleProp.width = tgtWidth + "px";
    styleProp.height = baseHeight + "px";
    createChartLayout(styleProp);
    // ---------------------------------------------
    styleProp.left = (baseLeft + tgtWidth) + "px";
    styleProp.width = (baseWidth - tgtWidth) + "px";
    createChartLayout(styleProp);
}

// 세로형 verticalType layout
var verticalTypeLayout = function(layoutId) {
//    console.log("세로형 verticalType ============layoutId " + layoutId);
    var hRate = baseStyle.heightRate;

    var baseLeft = $(layoutId).position().left;
    var baseTop = $(layoutId).position().top;
    var baseWidth = parseInt($(layoutId).css("width"));
    var baseHeight = parseInt($(layoutId).css("height"));

    var tgtHeight = Math.round(baseHeight * hRate);

    removeLayout(layoutId);

    styleProp.left = baseLeft + "px";
    styleProp.top = baseTop + "px";
    styleProp.width = baseWidth + "px";
    styleProp.height = tgtHeight + "px";
    createChartLayout(styleProp);
    // ---------------------------------------------
    styleProp.top = (tgtHeight + baseTop) + "px";
    styleProp.height = (baseHeight - tgtHeight) + "px";
    createChartLayout(styleProp);
}

// 신규 초기화
var initCls = "";
var layoutInit = function() {

    $("#basePanel").off("click", ".div-pan-layout");
    $(".div-pan-layout").remove();

    divCnt = 0;
    // 레이아웃 병합 편집
    $("#basePanel").on("click", ".div-pan-layout", function(event) {
//        console.log("#basePanel == " + event.currentTarget.id);
//        console.log("#basePanel target == " + event.target.id);
//        console.log("#basePanel target == " + $(this).attr("id"));
//        console.log("length = " + $("#" + event.currentTarget.id + "-chart").children().length);

        if ($("#basePanel").children(".div-pan-layout").length == 1) {
            event.stopPropagation();
            return;
        }

        if ($("#" + event.currentTarget.id + "-chart").children().length) {
            if (selectId != "") {
                $("#" + selectId).css("backgroundColor", "#ffffff");
            }
            selectId = "";
            event.stopPropagation();
            return;
        }

        var curPos = $(this).position();
        var curPosWidth = parseInt($(this).css("width"));
        var curPosHeigth = parseInt($(this).css("height"));

//        console.log(`curPos.left ${curPos.left} curPos.top ${curPos.top}  curPos.width ${curPosWidth} curPos.height ${curPosHeigth}`);

        if ($(this).attr("id") == selectId) {
            return;
        }

        //  selectId 이전 선택한 Id
        var confirmMessage = "선택하신 셀을 병합하시겠습니까?";
        if (selectId != "") {
//            console.log("selectId===================" + selectId);
            $("#" + selectId).css("backgroundColor", "#ffffff");
            var $sId = $("#" + selectId);
            var befPos = $sId.position();
            var befPosWidth = parseInt($sId.css("width"));
            var befPosHeigth = parseInt($sId.css("height"));

//            console.log(` befPos.left ${befPos.left} befPos.top ${befPos.top} befPos.width ${$sId.width()} befPos.height ${$sId.height()}`);

            if (curPos.left == befPos.left && curPosWidth == befPosWidth) {
//                console.log("세로형");
                if (curPos.top > befPos.top) {
//                    console.log("세로형 위쪽 > 아래쪽 선택");
                    if (curPos.top == (befPos.top + befPosHeigth)) {
                        if (confirm(confirmMessage)) {
                            $sId.remove();
                            $(this).css("top", befPos.top);
                            $(this).css("height", befPosHeigth + curPosHeigth);
                        }
                    }
                }
                else {
//                    console.log("세로형 아래쪽 > 위쪽  선택");
                    if (befPos.top == (curPos.top + curPosHeigth)) {
                        if (confirm(confirmMessage)) {
                            $sId.remove();
                            $(this).css("height", befPosHeigth + curPosHeigth);
                        }
                    }
                }
            }
            else if (curPos.top == befPos.top && curPosHeigth == befPosHeigth) {
//                console.log("가로형");
                if (curPos.left > befPos.left) {
//                    console.log("가로형 왼쪽 > 오른쪽 선택");
                    if (curPos.left == (befPos.left + befPosWidth)) {
                        if (confirm(confirmMessage)) {
                            $sId.remove();
                            $(this).css("left", befPos.left);
                            $(this).css("width", (curPosWidth + befPosWidth));
                        }
                    }
                }
                else {
//                    console.log("가로형 오른쪽 > 왼쪽 선택");
                    if (befPos.left == (curPos.left + curPosWidth)) {
                        if (confirm(confirmMessage)) {
                            $sId.remove();
                            $(this).css("width", (curPosWidth + befPosWidth));
                        }
                    }
                }
            }
        }
        selectId = $(this).attr("id");
//        console.log(selectId);
        $(this).css("backgroundColor", "#61e653");
    });

    if (initCls == "load") {
        return;
    }

    var $div = $("<div></div>");
    styleProp.left = baseStyle.left;
    styleProp.top = baseStyle.top;
    styleProp.width = baseStyle.width;
    styleProp.height = baseStyle.height;

    $div.css(styleProp);
    $div.addClass("div-pan-layout");
    $div.attr("id", "div-cell-" + divCnt);

    $div.append($("<div id='div-cell-" + divCnt + "-chart' class='div-chart-layout' ></div>"));
    var btnClone = $("#popModal_ex1").clone(true);
    $div.append(btnClone);

    $("#basePanel").append($div);
    divCnt++;
//    console.log(".div-pan-layout.length " + $("#basePanel").children(".div-pan-layout").length);
}

// 차트 선택
var selectChartId = "";
var chartInsertFunc = function(event) {
    selectChartId = event.currentTarget.id;

    $("#dialog-template").dialog("open");
    return;
}

// 레이아웃 Id, 템플릿Id,템플릿유형, 템플릿데이터
var selectChartInsert2 = function( layoutId, templateId, templateType, options) {
    // var chartId = selectChartId;
    var chartId = "#" + layoutId;
//    console.log("chartId  = " + chartId);

    var chartData = JSON.parse(options);

    $("#" + layoutId + "-chart").empty();

    chartManager($("#" + layoutId + "-chart").attr("id"));

    // 편집시 toolbar 감추기
    if (chartData.chart.hasOwnProperty("toolbar")) {
        if (chartData.chart.toolbar.hasOwnProperty("show")) {
            chartData.chart.toolbar.show = false;
        }
    }

    var chartObject = null;
    if (templateType == "list" ) {
//        console.log(chartData);
        chartObject = drawListChart(chartId + "-chart", chartData);
    }
    else {
        if ( templateType == "bar") {
            chartData.dataLabels.formatter   = (val) => { return val / 1000 + 'K' };
            if (chartData.hasOwnProperty('yaxis')) {
                chartData.yaxis.labels.formatter = (val) => { return val / 1000 + 'K' };
            }
        }
        else if ( templateType == "area") {
            for(var loop = 0; loop < chartData.series.length; loop++) {
                chartData.series[loop].data = generateDayWiseTimeSeries( new Date().getTime(), 20, {
                    min: 10,
                    max: 60
                });
            }

            if (chartData.hasOwnProperty('yaxis')) {
                chartData.yaxis.labels.formatter = (val) => { return (val / 1000000).toFixed(0) };
            }
            if (chartData.hasOwnProperty('tooltip.y')) {
                chartData.tooltip.y.formatter    = (val) => { return (val / 1000000).toFixed(0) };
            }
            
        }

        chartObject = drawChart(document.querySelector(chartId + "-chart"), chartData);
    }

    chartObject.opts.chart.chartId = templateId;
    chartObject.render();
    chartArray.push(chartObject);
//    console.dir(chartObject);
}

// 차트 생성기능 활성화
var chartInsert = function(chartId) {
    $("#" + chartId).on('click', chartInsertFunc);
}

// ==================================================
// 버튼 정의
// 신규 작성
$("#layout_new_btn").click(function() {
    $("#dialog-new-dashboard").dialog("open");
});

//  layout 유형 선택 팝업 버튼 .btn-setting
$(".btn-setting > #layout-select-btn").click(function(event) {
    // 팝업초기화
    baseStyle.widthRate = 0;
    baseStyle.heightRate = 0;
    // baseStyle.layoutObject = event;
    baseStyle.layoutObject = "#" + $(event.currentTarget).parent().parent().attr("id");
    baseStyle.layoutType = "";
/*
    console.log("layout-select-btn == " + event.currentTarget.id);
    console.log("parent.parent id  ==" + baseStyle.layoutObject);
    console.log("layout-select-btn == " + $(event.currentTarget).parent().attr("id"));
    console.log("layout-select-btn == " + $(event.currentTarget).parent().parent().attr("id"));
*/

    if (selectId != "") {
//        console.log("selectId===================" + selectId);
        $("#" + selectId).css("backgroundColor", "#ffffff");
        selectId = "";
    }


    var wVal = parseInt($(baseStyle.layoutObject).css("width"));
    var hVal = parseInt($(baseStyle.layoutObject).css("height"));
    var baseTableSize = 350; // max size 350px

    $("#vTable").css("width", baseTableSize + "px");  // 세로
    $("#vTable").css("height", baseTableSize + "px");
    $("#hTable").css("width", baseTableSize + "px");
    $("#hTable").css("height", baseTableSize + "px");

    if (wVal > hVal) {
        var tHeightRate = hVal / wVal;
//        console.log((baseTableSize * tHeightRate));
        $("#vTable").css("height", parseInt(baseTableSize * tHeightRate) + "px");
        $("#hTable").css("height", parseInt(baseTableSize * tHeightRate) + "px");

    }
    else {
        var tWidthRate = wVal / hVal;
//        console.log((baseTableSize * tWidthRate));
        $("#vTable").css("width", parseInt(baseTableSize * tWidthRate) + "px");
        $("#hTable").css("width", parseInt(baseTableSize * tWidthRate) + "px");
    }

    $("#dialog-layout-form").dialog("open");

    event.stopPropagation();
    return;
});


//  차트 선택 팝업 버튼 .btn-setting
$(".btn-setting > #chart-select-btn").click(function(event) {
//    console.log("layout-select-btn == " + event.currentTarget.id);
    selectChartId = $(event.currentTarget).parent().parent().attr("id");

//    console.log("selectChartId " + selectChartId);
    if (selectId != "") {
        $("#" + selectId).css("backgroundColor", "#ffffff");
    }
    $("#" + selectChartId).css("backgroundColor", "#ffffff");

    $("#dialog-template").dialog("open");
    event.stopPropagation();
});

// 휴지통버튼 생성된 차트 삭제기능
$(".btn-setting > #chart-delete-btn").click(function(event) {
//    console.log("click =====  #" + $(event.currentTarget).parent().parent().attr("id"));
//    console.log($(this).parent().prev().attr('id'));
//    console.log($(this).attr('id'));

    chartManager($(this).parent().prev().attr('id'));

    // 확인 팝업 생성
    $(this).parent().prev().empty(); // div-cell-?-chart
    event.stopPropagation();
});

// 레이아웃 저장
$("#layout_save_btn").click(function() {
//    console.log("layout_save_btn==================================");
    // var curId ="";
    if ( $(".div-pan-layout").length == 0 ) {
        return;
    }
    var layoutArray = [];
    var $layoutObj = null;

    dashboardMst.db_DASHBOARDSEQNO = $("#dashboardSeqNo").val();  // 0 이면 신규
    dashboardMst.db_DASHBOARDTYPE = $("#dashboardType").val();
    dashboardMst.db_REFRESHINTERVAL = $("#refreshInterval").val();
    dashboardMst.db_SCREENHEIGHT = $("#screenHeight").val();
    dashboardMst.db_SCREENWIDTH = $("#screenWidth").val();

    // 레이아웃
    for (var i = 0; i < $("#basePanel > .div-pan-layout").length; i++) {
        $layoutObj = $("#basePanel > .div-pan-layout")[i];
/*
        console.log($layoutObj.id);
        console.log($($layoutObj).css("width"), $($layoutObj).css("height"));
        console.log($($layoutObj).position().left, $($layoutObj).position().top);
        console.log("차트여부 " + $("#" + $layoutObj.id + "-chart").children().length);
        console.dir($($layoutObj));
*/
        var layoutInfo = {
            lo_LAYOUTID: $layoutObj.id,
            lo_LEFT: $($layoutObj).position().left,
            lo_TOP: $($layoutObj).position().top,
            lo_WIDTH: parseInt($($layoutObj).css("width")),
            lo_HEIGHT: parseInt($($layoutObj).css("height")),
            ci_CHARTID: "",
            ci_CHARTTYPE: ""
        }

        layoutArray.push(layoutInfo);
    }

//    console.log(layoutArray);
    // 차트 여부
    layoutArray.forEach(layoutObj => {
//        console.log(layoutObj.lo_LAYOUTID);

        chartArray.forEach(chartObj => {
            if (layoutObj.lo_LAYOUTID + "-chart" == chartObj.el.id) {
                layoutObj.ci_CHARTTYPE = chartObj.opts.chart.type;
                layoutObj.ci_CHARTID = chartObj.opts.chart.chartId;
            }
        });
    });

    dashboardMst.layoutInfoList = layoutArray;

//    console.log(JSON.stringify(dashboardMst, null, 4));

/*
    chartArray.forEach(element => {
        console.dir(element);
        console.log(element.el.id);
    });
*/

    $.ajax({
        type: 'POST',
        url: "/saveDashboard.se",
        async: true,
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        data: JSON.stringify(dashboardMst),
        success: function(result) {
/*
            console.dir(result);
            console.log(JSON.stringify(result));
            // result.unitList.forEach((element, idx) => {
            console.log("db_DASHBOARDSEQNO " + result.db_DASHBOARDSEQNO);

            result.layoutInfoList.forEach((element, idx) => {
                console.dir(element);
                console.log(element.ci_CHARTID, element.ci_CHARTTYPE, element.lo_LAYOUTID);
            });
*/            
            $("#dashboardSeqNo").val(result.db_DASHBOARDSEQNO); 
            messagePop("저장 되었습니다.");
        },
        error: function(request, status, error) {
//            console.log(error)
            messagePop("저장에 실패하였습니다.");
        }
    });

});
// 대시보드 불러오기
$("#dashboard_load_btn").click(function() {
    $("#dialog-dashboardSelect").dialog("open");
});

// 대시보드 다운로드
$("#dashboard_download_btn").click(function() {
//    location.href = "/sampleFile/GGG_36.html"

    if ( $("#dashboardSeqNo").val() == "" || parseInt($("#dashboardSeqNo").val()) == 0 ) {
        alert("저장 하신 후 사용하세요.");
        return;
    }
    dashboardMst.db_DASHBOARDSEQNO = $("#dashboardSeqNo").val();
    dashboardMst.db_DASHBOARDTYPE = $("#dashboardType").val();

    $.ajax({
        type: 'POST',
        url: "/dashboardDownload.se",
        cache: 'false',
        async: true,
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        data: JSON.stringify(dashboardMst),
        success: function(result) {
/*
            console.dir(result);
            console.log(JSON.stringify(result, null, 4));
*/
            location.href = "/sampleFile/" + dashboardMst.db_DASHBOARDTYPE + "_" +dashboardMst.db_DASHBOARDSEQNO + ".html";
        },
        error: function(request, status, error) {
            console.log(error)
        }
    });
    
});

// 미리보기
$("#dashboard_view_btn").click(function() {
    if ( $("#dashboardSeqNo").val() == "" || parseInt($("#dashboardSeqNo").val()) == 0 ) {
        alert("저장 하신 후 사용하세요.");
        return;
    }
    var screenWidth = $('#screenWidth').val();
    var screenHeight = $('#screenHeight').val();
    var win = window.open("/dashboardView.se?DB_DASHBOARDSEQNO=" + $("#dashboardSeqNo").val() , "preViewPopup", "width="+ (Number(screenWidth)+20) +", height="+ screenHeight);

});


$("#layout_chart_btn").click(function(event) {
    $("#dialog-chartManager").dialog("open");
});

// 사용자관리
$("#user_mng_btn").click(function() {
    location.href = "userDashboardMng.se";

});

/*
// 작성완료 버튼 (임시 삭제)
$("#complete_btn").click(function(event) {
    $("#basePanel").off("click", ".div-pan-layout");

    console.log(".div-pan-layout length " + $("#basePanel > .div-pan-layout").length);
    var imageStyle = {
        'background-image': 'url(images/insert_chart_FILL0_wght400.png)',
        'background-repeat': 'no-repeat',
        'background-position': 'center center'
    };

    for (var i = 0; i < $("#basePanel > .div-pan-layout").length; i++) {
        // $( $("#basePanel > .div-pan-layout")[i]).children(".btn-setting").remove();
        $($("#basePanel > .div-pan-layout")[i]).children(".btn-setting").empty();
        $($("#basePanel > .div-pan-layout")[i]).children(".btn-setting").off("click");

        $($("#basePanel > .div-pan-layout")[i]).css(imageStyle);
        $($("#basePanel > .div-pan-layout")[i]).css("backgroundColor", "#ffffff");

        var curId = $($("#basePanel > .div-pan-layout")[i]).attr("id");
        console.log("complete_btn curId " + curId);
        chartInsert(curId);
    }

});
*/

// 템플릿 선택
$("#templateContainer").on("click", ".chart-image", function(event) {
//    console.log( "templateContainer click" );
    selectId = "";
    $("#dialog-template").dialog("close");
//    console.log(templateResult);

    var layObj = templateResult.find((ele) => { return ele.ci_CHARTID == $(this).attr("id")});
//    console.log(layObj);
//    console.log(selectChartId);
    selectChartInsert2( selectChartId , layObj.ci_CHARTID, layObj.ci_CHARTTYPE , layObj.ci_CHARTOPTIONS );
    
//    console.log($(this).attr("id"));

});

// 대시보드 선택 및 삭제
$("#dashboardTable").on("click", "tbody td", function(event) {
//    console.log( $(event.target).text() );
//    console.log( $(this).parent().children().eq(0).text() );
    
    if ( event.target.cellIndex == 0 ) {
        $("#dialog-dashboardSelect").dialog("close");
        initCls = "load";
        removeChart();
        layoutInit();

        dashboardMst.db_DASHBOARDSEQNO = $(event.target).text();
    
        $.ajax({
            type: 'POST',
            url: "/layoutList.se",
            cache: 'false',
            async: true,
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            data: JSON.stringify(dashboardMst),
            success: function(result) {
//                console.dir(result);
//                console.log(JSON.stringify(result, null, 4));
    
                for (let key in dashboardMst) {
                    if ( key == "layoutInfoList") {
                        continue;
                    }
                    dashboardMst[key] = result[key];
                }
    
//                console.log(JSON.stringify(dashboardMst, null, 4));
    
                baseStyle.width = result.db_SCREENWIDTH + "px";
                baseStyle.height = result.db_SCREENHEIGH + "px";
                $("#screenWidth").val(result.db_SCREENWIDTH);
                $("#screenHeight").val(result.db_SCREENHEIGHT);
                $("#dashboardType").val(result.db_DASHBOARDTYPE);
                $("#refreshInterval").val(result.db_REFRESHINTERVAL);
                $("#dashboardSeqNo").val(result.db_DASHBOARDSEQNO);
    
                //console.log(result.layoutInfoList[0].ci_CHARTOPTIONS);
                divCnt = 0;
                var dNo = 0;
                var maxLayoutNo = 0;
                result.layoutInfoList.forEach((layObj, idx) => {
                    // selectId = layObj.layoutId;
                    styleProp.left   = layObj.lo_LEFT  + "px";
                    styleProp.top    = layObj.lo_TOP   + "px";
                    styleProp.width  = layObj.lo_WIDTH + "px";
                    styleProp.height = layObj.lo_HEIGHT + "px";
                    loadChartLayout(styleProp, layObj.lo_LAYOUTID);
                    
                    dNo = parseInt(layObj.lo_LAYOUTID.replace("div-cell-", ""));
//                    console.log("dNo = " + dNo);
                    if (maxLayoutNo <= dNo) {
                        maxLayoutNo = dNo;
                    }
                });
    
                divCnt = maxLayoutNo + 1;
    
                result.layoutInfoList.forEach((layObj, idx) => {
                    if ( layObj.ci_CHARTID ) {
                        selectChartId = layObj.lo_LAYOUTID;
                        selectChartInsert2( layObj.lo_LAYOUTID, layObj.ci_CHARTID, layObj.ci_CHARTTYPE , layObj.ci_CHARTOPTIONS );
                    }
                });
                
//                console.dir(chartArray);
                /*chartArray.forEach((element, idx) => {
                    element.render();
                });*/
    
            },
            error: function(request, status, error) {
                console.log(error)
            }
        });
    }
    else if ( event.target.cellIndex == 4 ) {
        // 미리보기
        if ( $(event.target).text() == "" ) {
            return;
        }
        var widthSize = (Number($(this).parent().children().eq(2).text()) + 20);
        var heightSize = Number($(this).parent().children().eq(3).text());
//        console.log('widthSize : ', widthSize);
//        console.log('heightSize : ', heightSize);
        var win = window.open("/dashboardView.se?DB_DASHBOARDSEQNO=" + $(this).parent().children().eq(0).text(), "dashboardViewPopup", "width="+ widthSize +",height="+ heightSize);
    }
    else if ( event.target.cellIndex == 8 ) {
        // 대시보드 삭제
        if (confirm( $(this).parent().children().eq(0).text() + " 번 보드를 삭제 하시겠습니까?")) {
            dashboardMst.db_DASHBOARDSEQNO = $(this).parent().children().eq(0).text();
            var $row = $(this).parent();
            $.ajax({
                type: 'POST',
                url: "/deleteDashboard.se",
                cache: 'false',
                async: true,
                contentType: "application/json; charset=utf-8",
                dataType: "json",
                data: JSON.stringify(dashboardMst),
                success: function(result) {
//                    console.dir( result );            
//                    console.log(JSON.stringify(result, null, 4));
                    if ( result > 0 ) {
                        deleteDashboardCnt++;
                        if ( dashboardMst.db_DASHBOARDSEQNO == $("#dashboardSeqNo").val() ) {
                            baseClear = true;    
                        }
                        $row.remove();
                    }
                },
                error: function(request, status, error) {
                    console.log(error)
                }
            });
        }
    }
});


// 템플릿관리 행 색상 반전
$("#templatetTable").on( "mouseover", "tbody tr", function() {
    $( this ).css( "background-color", "#f9ffcc" );
});

$("#templatetTable").on( "mouseleave","tbody tr", function() {
    $( this ).css( "background-color", "white" );
});

// 템플릿관리 선택
$("#templatetTable").on("click", "tbody td", function(event) {
//    console.log( $(event.target).text() );
//    console.log( $(this).parent().children().eq(0).text() );
    
    if ( event.target.cellIndex == 1 ) {
        initTemplateMng();
        chartInfo.ci_CHARTID = $(event.target).text();
        chartInfo.ci_CHARTTYPE = $(this).parent().children().eq(0).text()
    
        $.ajax({
            type: 'POST',
            url: "/templateSelect.se",
            cache: 'false',
            async: true,
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            data: JSON.stringify(chartInfo),
            success: function(result) {
//                console.dir(result);
//                console.log(JSON.stringify(result, null, 4));
                
                $("#chartTypeSelect").val(result[0].ci_CHARTTYPE);
                $("#chartId").val(result[0].ci_CHARTID);
                $("#chartImage").val(result[0].ci_CHARTIMAGE);
                
                $("#chartComment").val(result[0].ci_CHARTCOMMENT);
    
                $("#chartOptions").val(result[0].ci_CHARTOPTIONS);
                
                $("#chartTypeSelect").attr("disabled", true);
                $("#chartId").attr("disabled", true);
                
                if ( !result[0].ci_CHARTOPTIONS ) {
                    return;
                }
                var chartData = JSON.parse(result[0].ci_CHARTOPTIONS);

                if ( result[0].ci_CHARTTYPE == "list") {
                    var chart = new drawListChart("#chartPreview", chartData);
                    chart.render();
                }
                else {
                    if ( result[0].ci_CHARTTYPE == "bar") {
                        chartData.dataLabels.formatter   = (val) => { return val / 1000 + 'K' };
                        chartData.yaxis.labels.formatter = (val) => { return val / 1000 + 'K' };
                    }
                    else if ( result[0].ci_CHARTTYPE == "area") {
                        // chartData.yaxis.labels = { formatter:(val) => { return (val / 1000000).toFixed(0) } };
/*
                        if ( result[0].ci_CHARTID == "area_002") {
                            chartData.series[0].data = generateDayWiseTimeSeries( new Date().getTime(), 20, {
                                      min: 10,
                                      max: 60
                                    });
                            chartData.series[1].data = generateDayWiseTimeSeries( new Date().getTime(), 20, {
                                      min: 10,
                                      max: 20
                                    });
                            chartData.series[2].data = generateDayWiseTimeSeries( new Date().getTime(), 20, {
                                      min: 10,
                                      max: 15
                                    });
                            chartData.chart.events.selection = (chart, e) => { console.log(new Date(e.xaxis.min))};
                            console.log(chartData.series[0].data[0][1]);
                            console.log(chartData.series[1].data[0][1]);
                            console.log(chartData.series[2].data[0][1]);
                        }
                        else {
                            chartData.series[0].data = generateDayWiseTimeSeries( new Date().getTime(), 20, {
                                      min: 10,
                                      max: 60
                                    });
                            chartData.yaxis.labels.formatter = (val) => { return (val / 1000000).toFixed(0) };
                            chartData.tooltip.y.formatter    = (val) => { return (val / 1000000).toFixed(0) };
                        }
*/                   
                        for (var loop = 0; loop < chartData.series.length; loop++) {
                            chartData.series[loop].data = generateDayWiseTimeSeries( new Date().getTime(), 20, {
                                min: 10,
                                max: 60
                            });
                        }
                        if (chartData.hasOwnProperty('yaxis')) {
                            chartData.yaxis.labels.formatter = (val) => { return (val / 1000000).toFixed(0) };
                        }
                        if (chartData.hasOwnProperty('tooltip.y')) {
                            chartData.tooltip.y.formatter    = (val) => { return (val / 1000000).toFixed(0) };
                        }
                    }
                    var chart = new drawChart(document.querySelector("#chartPreview") , chartData);
                     chart.render();
                     templateObj = chart;
                }
            },
            error: function(request, status, error) {
                console.log(error)
            }
        });
    }
    
});

function generateDayWiseTimeSeries(baseval, count, yrange) {
   var i = 0;
   var series = [];
   while (i < count) {
     var x = baseval;
     var y = Math.floor(Math.random() * (yrange.max - yrange.min + 1)) + yrange.min;

     series.push([x, y]);
     baseval += 86400000;
     i++;
   }
   return series;
}
// ================ 레이아웃 유형선택 팝업 시작 ================
// 숫자만 입력
$("input:text[numberOnly]").on("keyup", function(event) {
    $(this).val($(this).val().replace(/[^0-9]/g, ""));

    if ($(event.target).val() == "") {
        return;
    }
    else {
        if (event.currentTarget.id == "vRate") {
            // $( $("#vTable tr")[0] ).children().eq(0).css("height", $(event.target).val() + "%");
            $($("#vTable tr")[0]).css("height", $(event.target).val() + "%");
        }
        else if (event.currentTarget.id == "hRate") {
            // console.log( event.currentTarget.id );
            $($("#hTable tr")[0]).children().eq(0).css("width", $(event.target).val() + "%");
//            console.log($(event.target).val());
        }
    }
});

var setLayoutType = function() {
    if ($("input:radio[name=layoutSelectType]:checked").val() == "vType") {
        $($("#vTable tr")[0]).children().eq(0).css("background-color", "#fcd76f");
        $($("#hTable tr")[0]).children().eq(0).css("background-color", "#9e9e9e");
        $("#vRate").attr("disabled", false);
        $("#hRate").attr("disabled", true);
        $("#vRate").focus();

    }
    else {
        $($("#vTable tr")[0]).children().eq(0).css("background-color", "#9e9e9e");
        $($("#hTable tr")[0]).children().eq(0).css("background-color", "#fcd76f");
        $("#hRate").attr("disabled", false);
        $("#vRate").attr("disabled", true);
        $("#hRate").focus();
    }
}
// 유형 선택 결과
var selectLayoutValue = function() {
    var retVal = layoutTypeRetVal;
    if ($("input:radio[name=layoutSelectType]:checked").val() == "vType") {
        retVal.layoutType = "vType";
        retVal.layOUtValue = $("#vRate").val()
        baseStyle.layoutType = retVal.layoutType;
        baseStyle.widthRate = parseInt(retVal.layOUtValue) * 0.01;
        baseStyle.heightRate = parseInt(retVal.layOUtValue) * 0.01;
        verticalTypeLayout(baseStyle.layoutObject);
    }
    else {
        retVal.layoutType = "hType";
        retVal.layOUtValue = $("#hRate").val()
        baseStyle.layoutType = retVal.layoutType;
        baseStyle.widthRate = parseInt(retVal.layOUtValue) * 0.01;
        baseStyle.heightRate = parseInt(retVal.layOUtValue) * 0.01;
        horizontalTypeLayout(baseStyle.layoutObject);
    }

    selectId = "";
//    console.log("selectLayoutValue " + retVal);
}

// 세로형 테이블 선택
$("#vTable").click(function(event) {
    $("input:radio[name=layoutSelectType][value='vType']").prop("checked", true);
    setLayoutType();

});
// 가로형 테이블 선택
$("#hTable").click(function(event) {
    $("input:radio[name=layoutSelectType][value='hType']").prop("checked", true);
    setLayoutType();
});

$("input:radio[name=layoutSelectType]").click(function() {
    setLayoutType();
});

$('#dialogModal_layout_form').click(function() {
    // 팝업초기화
    var tWidth = 200 * 0.1;  // 높이 고정값 200px
    $("#vTable").css("width", parseInt(tWidth) + "px");
    $("#hTable").css("width", parseInt(tWidth) + "px");

    $("#dialog-layout-form").dialog("open");
});

// ================ 팝업 정의 ================
// layout 유형 선택 팝업
$("#dialog-layout-form").dialog({
    autoOpen: false,
    height: 560,
    width: 730,
    modal: true,
    buttons: {
        "선택": function() {
            if ($("input:radio[name=layoutSelectType]").is(':checked')) {
                selectLayoutValue();
                $(this).dialog("close");
            }
            else {
                alert("선택된 레이아웃 유형이 없습니다.");
            }
        },
        "취소": function() {
            $(this).dialog("close");
        }
    },
    close: function() {
        
    }
});
// 대시보드 초기화
$("#dashboard-confirm").dialog({
    autoOpen: false,
    resizable: false,
    height: "auto",
    width: 400,
    modal: true,
    buttons: {
        "확인": function() {
            initDashboard();
            removeChart();
            
            $(this).dialog("close");
            $("#dialog-new-dashboard").dialog("open");
        },
        "취소": function() {
            $(this).dialog("close");
        },
    },
});

// 신규작성
$("#dialog-new-dashboard").dialog({
    autoOpen: false,
    resizable: false,
    height: 250,
    width: 400,
    modal: true,
    open: function(event, ui) {
        $("#inpDashboardType").val("");
        $("#inpScreenWidth").val("700");
        $("#inpScreenHeight").val("600");
        $("#inpReloadInt").val(10);
    },
    buttons: {
        "확인": function() {
            let setName = $.trim($("#inpDashboardType").val());
            if (setName == '') {
                messagePop("대시보드 이름을 입력하세요.");
                return;
            }
            let setWidth = $.trim($("#inpScreenWidth").val());
            if (Number(setWidth) == 0) {
                messagePop("화면크기 가로를 입력하세요. 최소 100, 최대 9999");
                return;
            }
            if (Number(setWidth) < 100 || Number(setWidth) > 9999) {
                messagePop("화면크기 가로를 확인하세요. 최소 100, 최대 9999");
                return;
            }
            let setHeight = $.trim($("#inpScreenHeight").val());
            if (Number(setHeight) == 0) {
                messagePop("화면크기 세로를 입력하세요. 최소 100, 최대 9999");
                return;
            }
            if (Number(setHeight) < 100 || Number(setHeight) > 9999) {
                messagePop("화면크기 세로를 확인하세요. 최소 100, 최대 9999");
                return;
            }
            let setReload = $.trim($("#inpReloadInt").val());
            if (Number(setReload) == 0) {
                messagePop("새로고침 간격을 입력하세요. 최소 1, 최대 999");
                return;
            }
            if (Number(setReload) < 1 || Number(setReload) > 999) {
                messagePop("새로고침 간격을 확인하세요. 최소 1, 최대 999");
                return;
            }
            initCls = "";
            initDashboard();
            
            $("#dashboardType").val(setName);
            $("#screenWidth").val(setWidth);
            $("#screenHeight").val(setHeight);
            $("#refreshInterval").val(setReload);
             
            baseStyle.width = $("#screenWidth").val() + "px";
            baseStyle.height = $("#screenHeight").val() + "px";

            drawGrid(); // 격자 처리
            removeChart();
            layoutInit();
            $(this).dialog("close");
        },
        "취소": function() {
            $(this).dialog("close");
        },
    },
});


// 템플릿 선택 팝업
$("#dialog-template").dialog({
    autoOpen: false,
    open: function(event, ui) {
        $("#templateContainer").empty();
        
        $.ajax({
            type: 'POST',
            url: "/templateList.se",
            cache: 'false',
            async: true,
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            data: JSON.stringify(chartInfo),
            success: function(result) {
//                console.dir(result);
//                console.log(JSON.stringify(result, null, 4));
                
                templateResult = result;
                $("#templatetTable tbody").empty();
                var tblHtml = "";
                result.forEach((rowObj, idx) => {
                    if ( rowObj.ci_CHARTIMAGE == "" || rowObj.ci_CHARTOPTIONS == "" ) {
                        tblHtml += '';
                    }
                    else {
                        tblHtml += '<div class="chart-icon" ><img class="chart-image" id="'+ rowObj.ci_CHARTID +'" title="'+ rowObj.ci_CHARTTYPE +'" ';
                        tblHtml += 'src="/images/dashboard/chartImage/'+ rowObj.ci_CHARTIMAGE +'">';
                        tblHtml += '<div class="chartId">'+ rowObj.ci_CHARTID +'</div>';
                        tblHtml += '</div>';
                    }
                });
                
                $("#templateContainer").append(tblHtml);
                
                var $images = $("#templateContainer .chart-icon");
                var length = $images.length;    // 이미지 갯수
        
                for (var i = 0; i < length; i++) {
                    var $img = $images.eq(i); // n번째 이미지 구하기
        
                    var x = (i % 3) * 210; // 위치 값 구하기 
                    var y = parseInt(i / 3) * 230;
        
                    $img.css({
                        left: x,
                        top: y
                    });
                }
            },
            error: function(request, status, error) {
                console.log(error);
                templateResult = []
            }
         });  
        
        
        
    },
    height: 600,
    width: 700,
    modal: true,
    buttons: {
        "취소": function() {
            $(this).dialog("close");
        }
    },
    close: function() {
        
    }
});


// 셀 병합 확인
$("#dialog-confirm-merge").dialog({
    autoOpen: false,
    resizable: false,
    height: "auto",
    width: 400,
    modal: true,
    buttons: {
        "확인": function() {
            $(this).dialog("close");
        },
        "취소": function() {
            $(this).dialog("close");
        },
    },
});

// 조회 항목 선택
$("#tableColumnChecker").dialog({
    autoOpen: false,
    height: 250,
    width: 700,
    modal: true,
    buttons: {
        "선택": function() {
            $(this).dialog("close");
            if ($('input:checkbox[name="col-checkbox"]:checked').length == 0) {
                return;
            }
            selectChartInsert("list_001");
        },
        "취소": function() {
            $(this).dialog("close");
        }
    },
    close: function() {

    }
});

// 대시보드 조회
$("#dialog-dashboardSelect").dialog({
    autoOpen: false,
    height: 500,
    width: 1073,
    modal: true,
    open : function() {
        deleteDashboardCnt = 0;
        baseClear = false;
        $.ajax({
            type: 'POST',
            url: "/dashboardSelect.se",
            cache: 'false',
            async: true,
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            data: JSON.stringify(dashboardMst),
            success: function(result) {
//                console.dir(result);
//                console.log(JSON.stringify(result, null, 4));
                $("#dashboardTable tbody").empty();
                var tblHtml = "";
                result.forEach((rowObj, idx) => {
                    tblHtml += '<tr style="height:25px">'
                    tblHtml += '<td width="50px" align="center"  class="linkText" >'+ rowObj.db_DASHBOARDSEQNO + '</td>';
                    tblHtml += '<td width="100px" >'+ rowObj.db_DASHBOARDTYPE + '</td>';
                    tblHtml += '<td width="60px"  align="right">'+ rowObj.db_SCREENWIDTH + '</td>';
                    tblHtml += '<td width="60px"  align="right">'+ rowObj.db_SCREENHEIGHT + '</td>';
                    if ( rowObj.as_CHARTLIST.length > 0 ) {
                        tblHtml += '<td width="80px"  align="center"  class="linkText" >미리보기</td>';    
                    }
                    else {
                        tblHtml += '<td width="80px" ></td>';
                    }
                    
                    
                    
                    tblHtml += '<td width="160px" align="center">'+ rowObj.db_REGDATE + '</td>';
                    tblHtml += '<td width="160px" align="center">'+ rowObj.db_FILEDATE + '</td>';
                    tblHtml += '<td width="280px" ><div style="width:270px">'+ rowObj.as_CHARTLIST + '</div></td>';
                    tblHtml += '<td width="60px"  align="center"  class="linkText" >삭제</td>';
                    tblHtml += '</tr>'
                
                });
                
                $("#dashboardTable tbody").append(tblHtml);
            },
            error: function(request, status, error) {
                console.log(error)
            }
        });  
    },
    buttons: {
        "닫기": function() {
            $(this).dialog("close");
        }
    },
    close: function() {
        if ( baseClear ) {
            initDashboard();
            removeChart();
            layoutInit();
        }
    }
});

// 템플릿 리스트
$("#dialog-chartManager").dialog({
    autoOpen: false,
    height: 800,
    width: 1500,
    modal: true,
    open : function() {
        initTemplateMng();
        
        $.ajax({
            type: 'POST',
            url: "/templateList.se",
            cache: 'false',
            async: true,
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            data: JSON.stringify(chartInfo),
            success: function(result) {
//                console.dir(result);
//                console.log(JSON.stringify(result, null, 4));
                $("#templatetTable tbody").empty();
                var tblHtml = "";
                result.forEach((rowObj, idx) => {
                    tblHtml += '<tr style="height:25px">'
                    tblHtml += '<td width="100px">'+ rowObj.ci_CHARTTYPE + '</td>';
                    tblHtml += '<td width="170px"  class="linkText" >'+ rowObj.ci_CHARTID + '</td>';
                    tblHtml += '</tr>'
                
                });
                
                $("#templatetTable tbody").append(tblHtml);
            },
            error: function(request, status, error) {
                console.log(error)
            }
         });           
    },
    buttons: {
        "신규": function() {
            initTemplateMng();
        },
        "저장": function() {
            chartInfo.ci_CHARTTYPE    = $("#chartTypeSelect").val();
            chartInfo.ci_CHARTID      = $("#chartId").val();
            chartInfo.ci_CHARTIMAGE   = $("#chartImage").val();
            chartInfo.ci_CHARTCOMMENT = $("#chartComment").val();
            chartInfo.ci_CHARTOPTIONS = $("#chartOptions").val();
            
            $.ajax({
                type: 'POST',
                url: $("#chartTypeSelect").attr("disabled") ?"/templateUpdate.se" :"/templateSave.se" ,
                cache: 'false',
                async: true,
                contentType: "application/json; charset=utf-8",
                dataType: "json",
                data: JSON.stringify(chartInfo),
                success: function(result) {
//                    console.log(JSON.stringify(result, null, 4));
                    if ( result.message ) {
                        alert(result.message);
                        return;
                    }
                    messagePop("저장 되었습니다.");
                    if ( !$("#chartTypeSelect").attr("disabled") ) {
                        var rowAdd = '<tr style="height:25px">'
                        rowAdd += '<td width="100px">'+ chartInfo.ci_CHARTTYPE + '</td>';
                        rowAdd += '<td width="170px" class="linkText" >'+ chartInfo.ci_CHARTID + '</td>';
                        rowAdd += '</tr>'

                        // $(rowAdd).insertAfter($("#templatetTable tr:last"));
                        $('#templatetTable > tbody:last').append(rowAdd);
                    }
                },
                error: function(request, status, error) {
//                    console.log(error)
                    messagePop("저장에 실패하였습니다.");
                }
            });
            
        },
        "닫기": function() {
            $(this).dialog("close");
        }
    },
    close: function() {
        initTemplateMng();
    }
});

// 로그아웃
$("#user_logOut_btn").click(function() {
 location.href = "/";

});
