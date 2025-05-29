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
    #container {
        display: flex;
        height: 50px;
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
        display: block;
        visibility: visible;
        height: auto;
        overflow: visible;
    }
</style>

</head>
<body>

<div id="container">
    <div id="tree"></div>

    <div id="policy-details">
        <div class="policy-header">선택된 정책의 세부 내용</div>
        <div id="policy-settings">
        </div>
    </div>
</div>

<script>
window.onload = function() {
    loadFiles();
};

async function loadFiles() {
    try {
        await getAdml();
        await getAdmx();
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

    policyNodes.forEach((policyNode) => {
        var name = policyNode.getAttribute('name');
        var displayNameKey = policyNode.getAttribute('displayName').replace('$(string.', '').replace(')', '');
        var displayName = admlData.stringTable[displayNameKey] || displayNameKey;
        var elements = [];

        var elementNodes = policyNode.querySelectorAll('elements > *');
        elementNodes.forEach((elementNode) => {
            var elementId = elementNode.getAttribute('id');
            var elementType = elementNode.getAttribute('type');
            var required = elementNode.getAttribute('required');
            var valueName = elementNode.getAttribute('valueName');

            elements.push({ elementId, elementType, required, valueName });
        });

        policies.push({ name, displayName, elements });
    });

    return { policies };
}

function buildTree() {
    var data = admxData.policies.map(policy => ({
        id: policy.name,
        text: policy.displayName,
        children: []
    }));

    $('#tree').jstree({
        'core': {
            'data': data
        }
    });

    $('#tree').on('select_node.jstree', function (e, data) {
        displayPolicyDetails(data.node.id);
    });
}

function displayPolicyDetails(policyId) {
    var policy = admxData.policies.find(p => p.name === policyId);

    if (policy) {
        var elementsHtml = policy.elements.map(el => `
            <div>
                <strong>요소 ID:</strong> ${el.elementId}<br>
                <strong>유형:</strong> ${el.elementType}<br>
                <strong>필수 여부:</strong> ${el.required}<br>
                <strong>값 이름:</strong> ${el.valueName}
            </div>
            <hr>
        `).join('');

        var policyDetailsHtml = `
            <div class="policy-header">${policy.displayName}</div>
            <div class="policy-divider"></div>
            ${elementsHtml}
        `;

        $('#policy-settings').html(policyDetailsHtml);
    } else {
        $('#policy-settings').html('<p>해당 정책을 찾을 수 없습니다.</p>');
    }
}
</script>

</body>
</html>