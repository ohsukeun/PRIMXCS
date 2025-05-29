<%@ page contentType="text/html; charset=utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="ui" uri="http://egovframework.gov/ctl/ui"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>
<%@ taglib prefix="spring" uri="http://www.springframework.org/tags"%>

<%
 /**
  * @Class Name : xmlparseSample.jsp
  * @Description : adml, admx 파일 파싱샘플
  * @Modification Information
  * 
  * @수정일      수정자          수정내용
  * ----------   --------       ---------------------------
  * 2024.06.30   개발자명        기존 코드 대체 및 스타일 개선
  *
  * @author 개발자명
  * @since 2024.06.30
  * @version 2.0
  */
%>

<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>PRIMX | xml parse sample</title>
<link rel="icon" href="<c:url value='/images/egovframework/com/primx/favicon.png' />">

<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/jquery-1.7.2.js' />"></script>

<link rel="stylesheet" href="<c:url value='/js/egovframework/com/primx/dist/themes/default/style.min.css' />" />
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/dist/jstree.js' />"></script>


</head>
<body>
	<div>adml, admx 파싱후 jsTree 생성하는 샘플</div>
	<div>1. ADML XML 데이터 처리</div>
	<div>
		<button type="button" onclick="getAdml();">adml XML 불러오기(서버)</button>
		<button type="button" onclick="getAdmlData();">adml XML 불러오기(직접입력)</button>
		
		<textarea id="admlData" style="width: 100%; height: 100px;" placeholder="adml xml 정보를 서버또는 직업 입력"></textarea>
		<br>
		<button type="button" onclick="viewAdmlData();">파싱된 adml 정보 보기</button>
		<textarea id="admlDataView" style="width: 100%; height: 100px;" placeholder="파싱된 adml 정보 표시함" readonly="readonly"></textarea>
		<br>
	</div>

<br>
	<div>2. ADMX XML 데이터 처리</div>
	<div>
		<button type="button" onclick="getAdmx();">admx XML 불러오기(서버)</button>
		<button type="button" onclick="getAdmxData();">admx XML 불러오기(직접입력)</button>
		
		<textarea id="admxData" style="width: 100%; height: 100px;" placeholder="admx xml 정보를 서버또는 직업 입력"></textarea>
		<br>
		<button type="button" onclick="viewAdmxData();">파싱된 admx 정보 보기</button>
		<textarea id="admxDataView" style="width: 100%; height: 100px;" placeholder="파싱된 admx 정보 표시함" readonly="readonly"></textarea>
		<br>
	</div>

<br>
	<button type="button" onclick="buildTree();">파싱된 admx 정보로 tree 구조 메뉴 생성</button>
    <div id="tree"></div>
    <br>
    
    <script>
	var admlData;
	function getAdml() {
		// XML 파일을 서버에서 가져와 파싱
		fetch('<c:url value="/js/ZoneCentral.adml" />')
			.then(response => response.text())  // 응답을 텍스트로 변환
			.then(xmlString => {
				$('#admlData').text(xmlString);
	    	  	admlData = parseAdmlData(xmlString);
		})
		.catch(error => console.error("Error fetching XML file:", error));
	}
	
	function getAdmlData() {
		// XML을 Textarea에서 가져와 파싱
		var xmlString = $('#admlData').val();
   	  	admlData = parseAdmlData(xmlString);
	}
	
	function viewAdmlData() {
		// 파싱된 데이터 표시
		if (admlData) {
			$('#admlDataView').val(JSON.stringify(admlData));
		} else {
			alert('adml 데이타 없음\nadml 불러오기후 진행');
		}
	}

	var admxData;
	function getAdmx() {
		if (!admlData) {
			alert('adml XML 파싱 정보없음\nadml 불러오기후 진행');
			return false;
		}

		fetch('<c:url value="/js/ZoneCentral.admx" />')
	    .then(response => response.text())  // 응답을 텍스트로 변환
	    .then(xmlString => {
	    	$('#admxData').text(xmlString);
	  	  	admxData = parseAdmxData(xmlString, admlData);
	    })
	    .catch(error => console.error("Error fetching XML 2 file:", error));
	}

	function getAdmxData() {
		if (!admlData) {
			alert('adml XML 파싱 정보없음\nadml 불러오기후 진행');
			return false;
		}
		// XML을 Textarea에서 가져와 파싱
		var xmlString = $('#admxData').val();
		admxData = parseAdmxData(xmlString, admlData);
	}
	
	function viewAdmxData() {
		if (!admlData) {
			alert('adml XML 파싱 정보없음\nadml 불러오기후 진행');
			return false;
		}
		// 파싱된 데이터 표시
		if (admxData) {
			$('#admxDataView').val(JSON.stringify(admxData));
		} else {
			alert('admx 데이타 없음\nadmx 불러오기후 진행');
		}
	}
	
    // adml parse
    function parseAdmlData(admlData) {
		var adml = {
			stringTable: {},
			presentationTable: {},
		};
		
		// dom parser
		var parser = new DOMParser();
		// xml data
		var xmlDoc = parser.parseFromString(admlData, 'application/xml');
		// String table parsing
		var stringNodes = xmlDoc.querySelectorAll('stringTable string');
		stringNodes.forEach((stringNode) => {
			var id = stringNode.getAttribute('id');
			var value = stringNode.textContent;
			adml.stringTable[id] = value;
		});

		// Presentation table parsing
		var presentationNodes = xmlDoc.querySelectorAll('presentationTable presentation');
		presentationNodes.forEach((presentationNode) => {
			var id = presentationNode.getAttribute('id');
			var content = presentationNode.innerHTML;
			adml.presentationTable[id] = content;
		});

		return adml;
	}

    // admx parse
	function parseAdmxData(admxData, admlData) {
		var policies = [];
		var categories = [];
		// dom parser
		var parser = new DOMParser();
		// xml data
		var xmlDoc = parser.parseFromString(admxData, 'application/xml');
		var policyNodes = xmlDoc.querySelectorAll('policy');

		// Enum table parsing
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

		// policy parsing
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

		// Category parsing
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

	var cateTreejs;
	function buildCategoryTreejs(categories, policies) {
		var rootCategories = [];

		// 카테고리
		categories.forEach((category) => {
			rootCategories.push({
				'id': category.name,
				'parent': category.parentCategory == 'primx:PrimX' ? '#' : category.parentCategory,
				'text': category.displayName,
				'explainText': category.explainText,
				'presentation': ''
			});
		});

		// 정책
		policies.forEach((policy) => {
			rootCategories.push({
				'id': policy.name,
				'parent': policy.parentCategory,
				'text': policy.displayName,
				'explainText': policy.explainText,
				'presentation': policy.presentation
			});
		});
		return rootCategories;
	};

	function buildTree() {
		if(!admxData) {
			alert('admx 데이타 없음\nadmx 불러오기후 진행');
			return false;
		}
		cateTreejs = buildCategoryTreejs(admxData.categories, admxData.policies);
		genTree(cateTreejs);
	}

	function genTree(data) {
		// 기존 jstree 있는경우 제거
		var oldTree = $('#tree').jstree(true);
		if (oldTree) {
			oldTree.destroy();
		}

		// jstree 생성
		$('#tree').jstree({
		    "core" : {
		        "themes" : {
		            "responsive": true
		        },
		      	//추가된 부분 시작
		        "data":data //추가된 부분 끝
		    },
		    "types" : {
		        "default" : {
		            "icon" : "fa fa-folder text-primary"
		        },
		        "file" : {
		            "icon" : "fa fa-file text-primary"
		        }
		    },
		    "plugins": ["types"]
		});
		
		// jstree 갱신
		$('#tree').jstree('refrash');
		
		// jstree 이벤트
		$('#tree').on('activate_node.jstree', function (e, data) {
		     if (data == undefined || data.node == undefined || data.node.id == undefined)
		                return;
		    alert('clicked node: ' + data.node.id);
		});
	}
    </script>
</body>
</html>
