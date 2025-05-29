
<%@ page contentType="text/html; charset=utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="ui" uri="http://egovframework.gov/ctl/ui"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib prefix="spring" uri="http://www.springframework.org/tags"%>

<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>PRIMX | xml parse sample</title>
<link rel="icon" href="<c:url value='/images/egovframework/com/primx/favicon.png' />">

<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/jquery-1.7.2.js' />"></script>

<link rel="stylesheet" href="<c:url value='/js/egovframework/com/primx/dist/themes/default/style.min.css' />" />
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/dist/jstree.js' />"></script>

<style>
    /* 레이아웃을 50:50으로 나누기 위한 스타일 */
    #container {
        display: flex;
        height: 500px;
    }

    #tree {
        width: 50%;
        border-right: 1px solid #ccc;
        height: 100%;
    }

    #policy-details {
        width: 50%;
        padding: 10px;
        height: 100%;
        overflow-y: auto;
    }

    .policy-header {
        font-size: 18px;
        font-weight: bold;
    }

    .policy-divider {
        margin: 10px 0;
        border-top: 1px solid #ccc;
    }

    .policy-settings {
        margin-top: 20px;
    }

    .policy-settings label {
        margin-right: 10px;
    }

    .policy-settings input {
        margin-right: 5px;
    }

    .policy-detail {
        margin-top: 10px;
        font-size: 14px;
    }
    
     #policy-settings { 
        display: block;   /* #policy-settings가 화면에 표시되도록 */
        visibility: visible;  /* visibility 확인 */
        height: auto;   /* 높이를 자동으로 설정 */
        overflow: visible;  /* 넘치는 부분도 보이게 설정 */
    }
</style>

</head>
<body>

<div id="container">
    <!-- 왼쪽: 트리 구조 -->
    <div id="tree"></div>

    <!-- 오른쪽: 세부 정책 내용 -->
    <div id="policy-details">
        <div class="policy-header">선택된 정책의 세부 내용</div>
        <div id="policy-settings">
            <!-- 선택된 정책의 세부 정보가 여기에 표시됩니다 -->
        </div>
    </div>
</div>

<script>
// 페이지 로드 시 자동으로 XML 파일을 불러옵니다.
window.onload = function() {
    loadFiles();  // ADML과 ADMX 파일을 순차적으로 불러오기
};

async function loadFiles() {
    try {
        await getAdml();  // ADML 파일을 서버에서 자동으로 불러오기
        await getAdmx();  // ADMX 파일을 서버에서 자동으로 불러오기
        await buildTree();
    } catch (error) {
        console.error("Error loading files:", error);
    }
}

var admlData;
async function getAdml() {
    const response = await fetch('<c:url value="/js/ZoneCentral.adml" />');
    const xmlString = await response.text();
    admlData = parseAdmlData(xmlString);
}

var admxData;
async function getAdmx() {
    if (!admlData) {
        alert('adml XML 파싱 정보없음\nadml 불러오기후 진행');
        return false;
    }

    const response = await fetch('<c:url value="/js/ZoneCentral.admx" />');
    const xmlString = await response.text();
    admxData = parseAdmxData(xmlString, admlData);
}

function parseAdmlData(admlData) {
    var adml = {
        stringTable: {},
        presentationTable: {},
    };
    
    var parser = new DOMParser();
    var xmlDoc = parser.parseFromString(admlData, 'application/xml');
    var stringNodes = xmlDoc.querySelectorAll('stringTable string');
    stringNodes.forEach((stringNode) => {
        var id = stringNode.getAttribute('id');
        var value = stringNode.textContent;
        adml.stringTable[id] = value;
    });

    var presentationNodes = xmlDoc.querySelectorAll('presentationTable presentation');
    presentationNodes.forEach((presentationNode) => {
        var id = presentationNode.getAttribute('id');
        var content = presentationNode.innerHTML;
        adml.presentationTable[id] = content;
    });

    return adml;
}

function parseAdmxData(admxData, admlData) {
	var policies = [];
    var categories = [];
    
    var parser = new DOMParser();
    var xmlDoc = parser.parseFromString(admxData, 'application/xml');
    var policyNodes = xmlDoc.querySelectorAll('policy');

    var enumTable = {};
    var enumNodes = xmlDoc.querySelectorAll('enum');
    enumNodes.forEach((enumNode) => {
        var id = enumNode.getAttribute('id');
        var items = Array.from(enumNode.querySelectorAll('item')).map((itemNode) => {
            var displayNameKey = itemNode.getAttribute('displayName').replace('$(string.', '').replace(')', '');
            var displayName = admlData.stringTable[displayNameKey] || displayNameKey;
            var valueNode = itemNode.querySelector('value decimal');
            var value = valueNode ? valueNode.getAttribute('value') : '';
            return { displayName, value };
        });
        enumTable[id] = items;
    });

    policyNodes.forEach((policyNode) => {
        var name = policyNode.getAttribute('name');
        var displayNameKey = policyNode.getAttribute('displayName').replace('$(string.', '').replace(')', '');
        var explainTextKey = policyNode.getAttribute('explainText').replace('$(string.', '').replace(')', '');
        var displayName = admlData.stringTable[displayNameKey] || displayNameKey;
        var explainText = admlData.stringTable[explainTextKey] || explainTextKey;
        var classType = policyNode.getAttribute('class');
        var presentationKey = policyNode.getAttribute('presentation').replace('$(presentation.', '').replace(')', '');
        var presentation = admlData.presentationTable[presentationKey] || null;
        var parentCategoryNode = policyNode.querySelector('parentCategory');
        var parentCategory = parentCategoryNode ? parentCategoryNode.getAttribute('ref') : null;
        var valueName = policyNode.getAttribute('valueName');
        var key = policyNode.getAttribute('key');

        policies.push({ name, displayName, explainText, classType, presentation, valueName, key, enumTable, parentCategory });
    });

    var categoryNodes = xmlDoc.querySelectorAll('categories category');
    categoryNodes.forEach((categoryNode) => {
        var name = categoryNode.getAttribute('name');
        var displayNameKey = categoryNode.getAttribute('displayName').replace('$(string.', '').replace(')', '');
        var explainTextKey = categoryNode.getAttribute('explainText').replace('$(string.', '').replace(')', '');
        var parentCategoryNode = categoryNode.querySelector('parentCategory');
        var parentCategory = parentCategoryNode ? parentCategoryNode.getAttribute('ref') : null;
        var displayName = admlData.stringTable[displayNameKey] || displayNameKey;
        var explainText = admlData.stringTable[explainTextKey] || explainTextKey;
        
        categories.push({ name, displayName, explainText, parentCategory, children: [] });
    });

    return { policies, categories };
}

function buildCategoryTreejs(categories, policies) {
    var rootCategories = [];

    // 'policyDefinitions' > 'categories' 항목을 트리 앞에 추가
    rootCategories.push({
        'id': 'policyDefinit        ions',
        'parent': '#',
        'text': 'policyDefinitions',
        'explainText': '',
        'presentation': ''
    });

    rootCategories.push({
        'id': 'categories',
        'parent': 'policyDefinitions',
        'text': 'categories',
        'explainText': '',
        'presentation': ''
    });

    categories.forEach((category) => {
        rootCategories.push({
            'id': category.name,
            'parent': 'categories',
            'text': category.displayName,
            'explainText': category.explainText,
            'presentation': ''
        });
    });

    policies.forEach((policy) => {
        rootCategories.push({
            'id': policy.name,
            'parent': policy.parentCategory || 'categories',
            'text': policy.displayName,
            'explainText': policy.explainText,
            'presentation': policy.presentation
        });
    });

    return rootCategories;
};

function buildTree() {
    if (!admxData) {
        alert('admx 데이터 없음\nadmx 불러오기 후 진행');
        return false;
    }
    var cateTreejs = buildCategoryTreejs(admxData.categories, admxData.policies);
    genTree(cateTreejs);
}

function genTree(data) {
    var oldTree = $('#tree').jstree(true);
    if (oldTree) {
        oldTree.destroy();
    }

    $('#tree').jstree({
        "core": {
            "themes": {
                "responsive": true
            },
            "data": data
        },
        "types": {
            "default": {
                "icon": "fa fa-folder text-primary"
            },
            "file": {
                "icon": "fa fa-file text-primary"
            }
        },
        "plugins": ["types"]
    });

    $('#tree').on('activate_node.jstree', function (e, data) {
    	//alert("Policy in FUN BEFOR: " + JSON.stringify(data.node.id, null, 2));
        if (data.node == undefined || data.node.id == undefined) return;
        displayPolicyDetails(data.node.id);
        
    });
}

function displayPolicyDetails(policyId) {
    var policy = admxData.policies.find(p => p.name === policyId);

    if (policy) {
        console.log("Policy details:", policy);  // 정책 객체를 출력하여 확인
        
        var enabledChecked = policy.enabledValue === '1' ? 'checked' : '';
        var disabledChecked = policy.disabledValue === '0' ? 'checked' : '';
        var notConfiguredChecked = policy.enabledValue !== '1' && policy.disabledValue !== '0' ? 'checked' : '';

        var supportedOn = policy.supportedOn || 'Not specified';  // Supported on 값을 확인하고 Not specified 처리

        // 템플릿 리터럴을 사용하여 HTML 생성
        var policyDetailsHtml = `
            <div class="policy-header">${policy.displayName || 'No Display Name Available'}</div>
            <div class="policy-divider"></div>
            <div><strong>Set title:</strong> ${policy.displayName || 'No Display Name Available'}</div>
            <div><strong>Supported on:</strong> ${supportedOn}</div>  
            <div class="policy-settings">
                <label>
                    <input type="radio" name="${policyId}" value="not_configured" ${notConfiguredChecked} /> Not configured
                </label>
                <label>
                    <input type="radio" name="${policyId}" value="enabled" ${enabledChecked}/> Enabled
                </label>
                <label>
                    <input type="radio" name="${policyId}" value="disabled" ${disabledChecked}/> Disabled
                </label>
            </div>
            <div><strong>Options:</strong> ${policy.valueName || 'Not available'}</div>
            <div class="policy-detail"><strong>Description:</strong> ${policy.explainText}</div>
        `;         
        console.log("HTML to insert:", policyDetailsHtml);  // 생성된 HTML을 출력하여 확인
        $('#policy-settings').html(policyDetailsHtml);  // HTML 업데이트
    } else {
        console.error("Policy not found for ID:", policyId);
        $('#policy-settings').html('<p>해당 정책을 찾을 수 없습니다.</p>');
    }
}
</script>

</body>
</html>
=======
<%@ page contentType="text/html; charset=utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="ui" uri="http://egovframework.gov/ctl/ui"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib prefix="spring" uri="http://www.springframework.org/tags"%>

<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>PRIMX | xml parse sample</title>
<link rel="icon" href="<c:url value='/images/egovframework/com/primx/favicon.png' />">

<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/jquery-1.7.2.js' />"></script>

<link rel="stylesheet" href="<c:url value='/js/egovframework/com/primx/dist/themes/default/style.min.css' />" />
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/dist/jstree.js' />"></script>

<style>
    /* 레이아웃을 50:50으로 나누기 위한 스타일 */
    #container {
        display: flex;
        height: 500px;
    }

    #tree {
        width: 50%;
        border-right: 1px solid #ccc;
        height: 100%;
    }

    #policy-details {
        width: 50%;
        padding: 10px;
        height: 100%;
        overflow-y: auto;
    }

    .policy-header {
        font-size: 18px;
        font-weight: bold;
    }

    .policy-divider {
        margin: 10px 0;
        border-top: 1px solid #ccc;
    }

    .policy-settings {
        margin-top: 20px;
    }

    .policy-settings label {
        margin-right: 10px;
    }

    .policy-settings input {
        margin-right: 5px;
    }

    .policy-detail {
        margin-top: 10px;
        font-size: 14px;
    }
    
     #policy-settings { 
        display: block;   /* #policy-settings가 화면에 표시되도록 */
        visibility: visible;  /* visibility 확인 */
        height: auto;   /* 높이를 자동으로 설정 */
        overflow: visible;  /* 넘치는 부분도 보이게 설정 */
    }
</style>

</head>
<body>

<div id="container">
    <!-- 왼쪽: 트리 구조 -->
    <div id="tree"></div>

    <!-- 오른쪽: 세부 정책 내용 -->
    <div id="policy-details">
        <div class="policy-header">선택된 정책의 세부 내용</div>
        <div id="policy-settings">
            <!-- 선택된 정책의 세부 정보가 여기에 표시됩니다 -->
        </div>
    </div>
</div>

<script>
// 페이지 로드 시 자동으로 XML 파일을 불러옵니다.
window.onload = function() {
    loadFiles();  // ADML과 ADMX 파일을 순차적으로 불러오기
};

async function loadFiles() {
    try {
        await getAdml();  // ADML 파일을 서버에서 자동으로 불러오기
        await getAdmx();  // ADMX 파일을 서버에서 자동으로 불러오기
        await buildTree();
    } catch (error) {
        console.error("Error loading files:", error);
    }
}

var admlData;
async function getAdml() {
    const response = await fetch('<c:url value="/js/ZoneCentral.adml" />');
    const xmlString = await response.text();
    admlData = parseAdmlData(xmlString);
}

var admxData;
async function getAdmx() {
    if (!admlData) {
        alert('adml XML 파싱 정보없음\nadml 불러오기후 진행');
        return false;
    }

    const response = await fetch('<c:url value="/js/ZoneCentral.admx" />');
    const xmlString = await response.text();
    admxData = parseAdmxData(xmlString, admlData);
}

function parseAdmlData(admlData) {
    var adml = {
        stringTable: {},
        presentationTable: {},
    };
    
    var parser = new DOMParser();
    var xmlDoc = parser.parseFromString(admlData, 'application/xml');
    var stringNodes = xmlDoc.querySelectorAll('stringTable string');
    stringNodes.forEach((stringNode) => {
        var id = stringNode.getAttribute('id');
        var value = stringNode.textContent;
        adml.stringTable[id] = value;
    });

    var presentationNodes = xmlDoc.querySelectorAll('presentationTable presentation');
    presentationNodes.forEach((presentationNode) => {
        var id = presentationNode.getAttribute('id');
        var content = presentationNode.innerHTML;
        adml.presentationTable[id] = content;
    });

    return adml;
}

function parseAdmxData(admxData, admlData) {
	var policies = [];
    var categories = [];
    
    var parser = new DOMParser();
    var xmlDoc = parser.parseFromString(admxData, 'application/xml');
    var policyNodes = xmlDoc.querySelectorAll('policy');

    var enumTable = {};
    var enumNodes = xmlDoc.querySelectorAll('enum');
    enumNodes.forEach((enumNode) => {
        var id = enumNode.getAttribute('id');
        var items = Array.from(enumNode.querySelectorAll('item')).map((itemNode) => {
            var displayNameKey = itemNode.getAttribute('displayName').replace('$(string.', '').replace(')', '');
            var displayName = admlData.stringTable[displayNameKey] || displayNameKey;
            var valueNode = itemNode.querySelector('value decimal');
            var value = valueNode ? valueNode.getAttribute('value') : '';
            return { displayName, value };
        });
        enumTable[id] = items;
    });

    policyNodes.forEach((policyNode) => {
        var name = policyNode.getAttribute('name');
        var displayNameKey = policyNode.getAttribute('displayName').replace('$(string.', '').replace(')', '');
        var explainTextKey = policyNode.getAttribute('explainText').replace('$(string.', '').replace(')', '');
        var displayName = admlData.stringTable[displayNameKey] || displayNameKey;
        var explainText = admlData.stringTable[explainTextKey] || explainTextKey;
        var classType = policyNode.getAttribute('class');
        var presentationKey = policyNode.getAttribute('presentation').replace('$(presentation.', '').replace(')', '');
        var presentation = admlData.presentationTable[presentationKey] || null;
        var parentCategoryNode = policyNode.querySelector('parentCategory');
        var parentCategory = parentCategoryNode ? parentCategoryNode.getAttribute('ref') : null;
        var valueName = policyNode.getAttribute('valueName');
        var key = policyNode.getAttribute('key');

        policies.push({ name, displayName, explainText, classType, presentation, valueName, key, enumTable, parentCategory });
    });

    var categoryNodes = xmlDoc.querySelectorAll('categories category');
    categoryNodes.forEach((categoryNode) => {
        var name = categoryNode.getAttribute('name');
        var displayNameKey = categoryNode.getAttribute('displayName').replace('$(string.', '').replace(')', '');
        var explainTextKey = categoryNode.getAttribute('explainText').replace('$(string.', '').replace(')', '');
        var parentCategoryNode = categoryNode.querySelector('parentCategory');
        var parentCategory = parentCategoryNode ? parentCategoryNode.getAttribute('ref') : null;
        var displayName = admlData.stringTable[displayNameKey] || displayNameKey;
        var explainText = admlData.stringTable[explainTextKey] || explainTextKey;
        
        categories.push({ name, displayName, explainText, parentCategory, children: [] });
    });

    return { policies, categories };
}

function buildCategoryTreejs(categories, policies) {
    var rootCategories = [];

    // 'policyDefinitions' > 'categories' 항목을 트리 앞에 추가
    rootCategories.push({
        'id': 'policyDefinitions',
        'parent': '#',
        'text': 'policyDefinitions',
        'explainText': '',
        'presentation': ''
    });

    rootCategories.push({
        'id': 'categories',
        'parent': 'policyDefinitions',
        'text': 'categories',
        'explainText': '',
        'presentation': ''
    });

    categories.forEach((category) => {
        rootCategories.push({
            'id': category.name,
            'parent': 'categories',
            'text': category.displayName,
            'explainText': category.explainText,
            'presentation': ''
        });
    });

    policies.forEach((policy) => {
        rootCategories.push({
            'id': policy.name,
            'parent': policy.parentCategory || 'categories',
            'text': policy.displayName,
            'explainText': policy.explainText,
            'presentation': policy.presentation
        });
    });

    return rootCategories;
};

function buildTree() {
    if (!admxData) {
        alert('admx 데이터 없음\nadmx 불러오기 후 진행');
        return false;
    }
    var cateTreejs = buildCategoryTreejs(admxData.categories, admxData.policies);
    genTree(cateTreejs);
}

function genTree(data) {
    var oldTree = $('#tree').jstree(true);
    if (oldTree) {
        oldTree.destroy();
    }

    $('#tree').jstree({
        "core": {
            "themes": {
                "responsive": true
            },
            "data": data
        },
        "types": {
            "default": {
                "icon": "fa fa-folder text-primary"
            },
            "file": {
                "icon": "fa fa-file text-primary"
            }
        },
        "plugins": ["types"]
    });

    $('#tree').on('activate_node.jstree', function (e, data) {
    	alert("Policy in FUN BEFOR: " + JSON.stringify(data.node.id, null, 2));
        if (data.node == undefined || data.node.id == undefined) return;
        displayPolicyDetails(data.node.id);
        
    });
}

function displayPolicyDetails(policyId) {
    var policy = admxData.policies.find(p => p.name === policyId);

    if (policy) {
        console.log("Policy details:", policy);  // 정책 객체를 출력하여 확인
        
        var enabledChecked = policy.enabledValue === '1' ? 'checked' : '';
        var disabledChecked = policy.disabledValue === '0' ? 'checked' : '';
        var notConfiguredChecked = policy.enabledValue !== '1' && policy.disabledValue !== '0' ? 'checked' : '';

        // Supported on 값을 확인하고 Not specified로 처리
        var supportedOn = policy.supportedOn ? policy.supportedOn : 'Not specified';  // 수정된 부분
        //alert("supportedOn in: " + supportedOn); 
        
        alert("policy.displayName : " + policy.displayName); 
        var policyDetailsHtml = `
            <div class="policy-header">${policy.name} - ${policy.displayName}</div>
            <div class="policy-divider"></div>
            <div><strong>Set title:</strong> ${policy.displayName}</div>
            <div><strong>Supported on:</strong> ${supportedOn}</div>  <!-- 수정된 부분 -->
            <div class="policy-settings">
                <label>
                    <input type="radio" name="${policyId}" value="not_configured" ${notConfiguredChecked} /> Not configured
                </label>
                <label>
                    <input type="radio" name="${policyId}" value="enabled" ${enabledChecked}/> Enabled
                </label>
                <label>
                    <input type="radio" name="${policyId}" value="disabled" ${disabledChecked}/> Disabled
                </label>
            </div>
            <div><strong>Options:</strong> ${policy.valueName}</div>
            <div class="policy-detail"><strong>Description:</strong> ${policy.explainText}</div>
        `;
        
        $('#policy-settings').html(policyDetailsHtml);  // HTML 업데이트
    } else {
        console.error("Policy not found for ID:", policyId);
        $('#policy-settings').html('<p>해당 정책을 찾을 수 없습니다.</p>');
    }
}
</script>

</body>
</html>
