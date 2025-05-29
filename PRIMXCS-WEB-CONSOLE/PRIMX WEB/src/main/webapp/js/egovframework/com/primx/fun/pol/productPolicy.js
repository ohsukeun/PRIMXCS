let originalPolicyData = []; // 원본 정책 데이터 저장용 배열
let defaultPolicyData = []; // 기본 정책 데이터 저장용 배열
let copyPolicyData = []; // 복사 정책 데이터 저장용 배열


// 고정 정책 배열 정의
const fixedPolicyIds = [120, 121, 122, 127, 309, 343, 345, 131, 499, 2, 197, 392, 103, 105, 123, 124, 125, 126, 5120, 302, 306, 307, 308];
const safetyZonePolicyIds = [133, 134, 135];

// 일부 정책은 빈 값 허용
var emptyPolicyIds = [133, 134];

/* 정책 > 제품 정책 */
/* csv 다운로드 제목포멧 */
var firstDepthTit = $(".left nav .firstDepthMenu > li.select > p").text();
var secondDepthTit = $(".left nav .firstDepthMenu > li.select .secondDepthMenu > li.select a").text();
var today = new Date();
var formattedDate = today.getFullYear() + '-' +
    ('0' + (today.getMonth() + 1)).slice(-2) + '-' +
    ('0' + today.getDate()).slice(-2);

var productPolicyTable;
var polSysStatusTable;
var polSysStatusAddPopupTable;

function pageInit(){
	/* 팝업공통 */
	commonPopup();
	/* 커스텀 드롭 메뉴 온 오프 */ 
	dropMenu();

	/* 데이터 테이블 */	
	loadProductPolicyTable();
	
    //loadPolicyData();
    loadMandatoryPolicy();

	/* datepicker */
	datePicker();

    toggleSetting();

    
	/* 왼쪽메뉴 셀렉트 되도록 */
	var firstDepth = $(".firstDepthMenu .policy");
	$(".left li").removeClass("select");
	$(firstDepth).closest("li").addClass("select open");
	$(".productPolicyLi").addClass("select");
}

function loadProductPolicyTable(){
    // 기존 테이블이 있다면 제거
	if (productPolicyTable) {
		productPolicyTable.destroy();
		productPolicyTable = null;
	}

	// 기존 CSV 버튼 이벤트 바인딩 제거
	$('#productPolicyCsvExport').off('click');
	console.log("loadProductPolicyTable");
	productPolicyTable = initDataTables("#productPolicyTable", {
		processing: true,
		serverSide: true,
		ajax: {
			url: '/primx/fun/pol/product/policy/list.do',
			type: 'POST',
			dataSrc: 'resultList',
			data: function(d) {
				d.searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']").val();
				d.searchStartDate = $(".searchAreaPopup .search_wrap input[name='startDt']").val();
				d.searchEndDate = $(".searchAreaPopup .search_wrap input[name='endDt']").val();
				d.searchPolicyType = $(".searchAreaPopup .search_wrap select[name='searchPolicyType']").val();
				d.searchProduct = $(".searchAreaPopup .search_wrap select[name='searchProduct']").val();
				d.searchCondition = $(".searchAreaPopup .search_wrap select[name='searchCondition']").val();
				d.searchKeyword = $(".searchAreaPopup .search_wrap input[name='searchKeyword']").val();
			}
		},
		columns: [
			{ 
				data: null, 
				orderable: false, // 정렬 비활성화
				className: "text-center",
				render: function(data, type, row, meta) {
					if (type === "display") {
						return `<input type="checkbox" name="rowSelect" value="${row.policyCode}" class="row-checkbox" />`;
					}
					return "";
				}
			},
			{ data: null, render: function(data, type, row, meta) {
				return meta.row + 1; // 행 번호 자동 생성
			}},
            { 
				data: "isDefault",
				render: function(data, type, row) {
					return data === 'Y' ? '<span class="defaultPol">기본</span>' : '<span class="eachPol">개별</span>';
				},
				createdCell: function (td, cellData, rowData, row, col) {
			        $(td).addClass('result');
			    }
			},
            { data: "productName" },
			{ data: "policyCode" },
			{ data: "policyName" },
			{ data: "systemMapCount" },
			{ data: "createdByNm" },
			{ 
				data: "createdDate",
				render: function(data) {
                    // null 일 경우 빈 문자열 반환
                    if(data === null){
                        return '';
                    }
					return new Date(data).toLocaleString();
				}
			},
			{ 
				data: "modifiedDate",
				render: function(data) {
                    // null 일 경우 빈 문자열 반환
                    if(data === null){
                        return '';
                    }
					return new Date(data).toLocaleString();
				}
			}
		],
		columnDefs: [
			{
				targets: 0, // 첫 번째 열
				orderable: false, // 정렬 비활성화
				className: "text-center"
			}
		],
		buttons: [
			{
				extend: 'csv',
				title: `${firstDepthTit}_${secondDepthTit}_${formattedDate}`,
				exportOptions: {
					columns: ':not(:first-child)'  // 첫 번째 컬럼(체크박스)을 제외한 모든 컬럼 내보내기
				},
				bom: true, // UTF-8 BOM 추가
				customize: function(csv) {
					return '\ufeff' + csv; // UTF-8 BOM 추가하여 한글 깨짐 방지
				}
			}
		]
	});

    // CSV 내보내기 버튼과 연결
    $('#productPolicyCsvExport').on('click', function() {
        productPolicyTable.button('.buttons-csv').trigger();
    });

	// 단일 선택 체크박스 이벤트
	$('#productPolicyTable tbody').on('change', '.row-checkbox', function() {
		$('.row-checkbox').not(this).prop('checked', false); // 다른 체크박스 해제
		$('#productPolicyTable tbody tr').removeClass('selected'); // 모든 행에서 선택 효과 제거

		if ($(this).prop('checked')) {
			$(this).closest('tr').addClass('selected'); // 선택된 행에 배경색 적용
		}
	});

    // productPolicyTable 체크박스 외 다른 부분 클릭 시 팝업 열기
    $('#productPolicyTable tbody').on('click', 'td:not(:first-child)', function() {
        var selectedRow = $(this).closest('tr');
        var selectedPolicyInfo = selectedRow.find('td');
        // 매개변수로 row 전달
        viewPopup(selectedPolicyInfo);
    });
}

async function viewPopup(selectedRow){
    popupPolicyClear();
    
    // 수정버튼 보이기
    $('#updateBtn').removeClass('hide');

    // 제품 가져오기
    var product = selectedRow.eq(3).text();
    var productInput = $('#productSelect');
    // productInput의 값이 product인 것으로 선택
    productInput.val(product).prop('selected', true);
    console.log('product: ', product);

    // 정책 유형 가져오기
    var policyType = selectedRow.eq(2).text();
    var policyTypeInput = $('#policyTypeInput');
    policyTypeInput.val(policyType);
    console.log('policyType: ', policyType);

    // 정책 명 가져오기
    var policyName = selectedRow.eq(5).text();
    console.log('policyName: ', policyName);
    var policyNameInput = $('#policyNameInput');
    policyNameInput.val(policyName);
    console.log('policyNameInput: ', policyNameInput);

    // 정책 ID 가져오기
    var policyCode = selectedRow.eq(4).text();
    console.log('policyCode: ', policyCode);
    var policyCodeInput = $('#policyCodeInput');
    policyCodeInput.val(policyCode);
    console.log('policyCodeInput: ', policyCodeInput);

    try {
        await loadDefaultPolicy();

        const copyPolicy = await getPolicyInfo(policyCode);
        console.log('policyInfo: ', copyPolicy);
        
        defaultPolicySetting(copyPolicy.policyDetailList, false);

        // 저장버튼 숨기기
        $('#saveBtn').addClass('hide');
        $('.productBasePolicyPopup').removeClass('hide');
    } catch(error) {
        console.error('정책 정보 가져오기 실패:', error);
    }
    
    // 팝업 스크롤 최상단으로 이동
    $('.productBasePolicyPopup .pop_cont .divisionWrap > div').scrollTop(0);
}

/*정책 셀렉트 값에 따른 영역 숨김*/
$(".polSelectBox .drop").change(function () {
    var selectedValue = $(this).val();
    var allPolicy = $(".polSection");

    // 선택한 값에 따라 hide 클래스 제거
    if (selectedValue === "fix") {
		$(allPolicy).addClass("hide");
        $(".fixedPol").removeClass("hide");
    } else if (selectedValue === "132") {
		$(allPolicy).addClass("hide");
        $(".pol132").removeClass("hide");
    } else if (selectedValue === "354") {
		$(allPolicy).addClass("hide");
        $(".pol354").removeClass("hide");
    } else if (selectedValue === "153") {
		$(allPolicy).addClass("hide");
        $(".pol153").removeClass("hide");
    }
});

function toggleSetting(){
    // default132Checkbox, default354Checkbox, default153Checkbox 체크박스 토글
    
    // 1. default132Checkbox 하위에 있는 select 및 input disabled 처리
    toggleInputs(document.getElementById("default132Checkbox"), "pol132");
    toggleInputsAndTable(document.getElementById("default132Checkbox"), "pol132");

    // 2. default354Checkbox 하위에 있는 리스트 테이블 disabled 처리
    toggleInputsAndTable(document.getElementById("default354Checkbox"), "pol354");

    // 3. default153Checkbox 하위에 있는 select 및 input disabled 처리
    toggleCheckboxes(document.getElementById("default153Checkbox"), "pol153");
}

function toggleInputs(checkbox, sectionClass) {
    const section = document.querySelector("." + sectionClass);
    const inputs = section.querySelectorAll('input[type="text"]');
    const selects = section.querySelectorAll('select');
    
    checkbox.addEventListener("change", function () {
        inputs.forEach(input => {
            input.disabled = !checkbox.checked;
        });
        selects.forEach(select => {
            select.disabled = !checkbox.checked;
        });
    });
}

function toggleCheckboxes(checkbox, sectionClass) {
    const section = document.querySelector("." + sectionClass);
    const checkboxes = section.querySelectorAll('tbody input[type="checkbox"]');

    checkbox.addEventListener("change", function () {
        checkboxes.forEach(input => {
            input.disabled = !checkbox.checked;
        });
    });
}

function toggleInputsAndTable(checkbox, sectionClass) {
    const section = document.querySelector("." + sectionClass);
    const inputs = section.querySelectorAll('input[type="text"]');
    const table = section.querySelector('table');
    
    checkbox.addEventListener("change", function () {
        // 테이블의 모든 행에서 input 요소들을 찾아서 disabled 속성을 토글
        const tableRows = table.querySelectorAll('tbody tr');
        tableRows.forEach(row => {
            const rowInputs = row.querySelectorAll('input[type="text"]');
            rowInputs.forEach(input => {
                input.disabled = !checkbox.checked;
            });
        });

        // 일반 input 요소들의 disabled 속성도 토글
        inputs.forEach(input => {
            input.disabled = !checkbox.checked;
        });
    });
}



/* 정책 관련 */
var admlData;
var admxData;

async function loadPolicyData() {
    console.log('loadPolicyData');
    await getAdml();
    console.log('admlData: ', admlData);
    await getAdmx();
    console.log('admxData: ', admxData);

    buildTree();
}

async function getAdml() {
    const response = await fetch("/js/ZoneCentral.adml");
    const xmlString = await response.text();
    admlData = parseAdmlData(xmlString);
}

async function getAdmx() {
    if (!admlData) {
        alert('adml XML 파싱 정보없음\nadml 불러오기후 진행');
        return false;
    }

    const response = await fetch("/js/ZoneCentral.admx");
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

         var elements = [];
        var elementNodes = policyNode.querySelectorAll('elements > *');
        elementNodes.forEach((elementNode) => {
            var elementId = elementNode.getAttribute('id');
            var elementType = elementNode.tagName;
            var required = elementNode.getAttribute('required');
            var valueName = elementNode.getAttribute('valueName');

            var items = [];
            if (elementType === 'enum') {
                var itemNodes = elementNode.querySelectorAll('item');
                itemNodes.forEach((itemNode) => {
                    var itemDisplayNameKey = itemNode.getAttribute('displayName').replace('$(string.', '').replace(')', '');
                    var itemDisplayName = admlData.stringTable[itemDisplayNameKey] || itemDisplayNameKey;
                    var value = itemNode.querySelector('decimal')?.getAttribute('value') || 'N/A';
                    items.push({ itemDisplayName, value });
                });
            }

            elements.push({ elementId, elementType, required, valueName, items });
        });

        policies.push({ name, displayName, explainText, classType, presentation, valueName, key, enumTable, parentCategory, elements });
    });

    // 🔹 Category 노드 파싱
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
    var categoryMap = {}; // 상위 카테고리를 찾기 위한 맵

    // 최상위 루트 노드: ZONECENTRAL (한 번만 추가)
    rootCategories.push({
        'id': 'ZONECENTRAL',
        'parent': '#',  // 최상위 노드
        'text': 'ZONECENTRAL',
        'explainText': '',
        'presentation': '',
        'type': 'default'
    });

    categories.forEach((category) => {
        if (category.name !== 'ZONECENTRAL' && category.name !== 'CatProduct') {
            let parentCategory = 'ZONECENTRAL'; // 기본 부모

            // 숫자로 시작하는 하위 카테고리는 상위 카테고리 아래 배치
            const match = category.displayName.match(/^(\d+)\.(\d+)/); // 3.1, 3.2 형태 찾기
            if (match) {
                let parentPrefix = match[1]; // 상위 번호 추출 (예: "3.1" → "3")
                let possibleParent = Object.keys(categoryMap).find(id => categoryMap[id].startsWith(parentPrefix));
                if (possibleParent) {
                    parentCategory = possibleParent;
                }
            }

            rootCategories.push({
                'id': category.name,
                'parent': parentCategory,
                'text': category.displayName,
                'explainText': category.explainText,
                'presentation': '',
                'type': 'default'
            });

            categoryMap[category.name] = category.displayName; // 카테고리 맵에 저장
        }
    });

    policies.forEach((policy) => {
        let parentCategory = policy.parentCategory;

        // "CatProduct"를 부모로 가지는 정책들은 "ZONECENTRAL" 하위로 바로 이동
        if (parentCategory === 'CatProduct' || parentCategory === 'ZONECENTRAL' || !parentCategory) {
            parentCategory = 'ZONECENTRAL'; // 부모가 없으면 ZONECENTRAL 바로 아래로 이동
        }

        rootCategories.push({
            'id': policy.name,
            'parent': parentCategory,  // 올바른 카테고리 아래 배치
            'text': policy.displayName,
            'explainText': policy.explainText,
            'presentation': policy.presentation,
            'type': 'file'
        });
    });

    console.log("Final Tree Structure:", rootCategories); // 디버깅용 로그

    return rootCategories;
}




function buildTree() {
    if (!admxData) {
        alert('admx 데이터 없음\nadmx 불러오기 후 진행');
        return false;
    }
    var cateTreejs = buildCategoryTreejs(admxData.categories, admxData.policies);
    console.log('cateTreejs: ', cateTreejs);
    genTree(cateTreejs);
}

function genTree(data) {
    var oldTree = $('#tree').jstree(true);

    // 기존 트리를 완전히 제거하고 초기화
    if (oldTree) {
        $('#tree').jstree("destroy").empty();
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
                "icon": "../../../../../images/egovframework/com/primx/folder-pol.svg"
            },
            "file": {
                "icon": "../../../../../images/egovframework/com/primx/file-pol.svg"
            }
        },
        "plugins": ["types"]
    }).on("select_node.jstree", function (e, data) {
        var selectedNode = data.node;
        if (selectedNode.type === 'file') {
            displayPolicyDetails(selectedNode.id);
        }
    });

    // 트리 데이터 콘솔 출력 (디버깅용)
    console.log("Tree Data Rendered:", data);
}

function displayPolicyDetails(policyId) {
    var policy = admxData.policies.find(p => p.name === policyId);

    // 🔹 폴더 클릭 방지
    var clickedNode = $('#tree').jstree(true).get_node(policyId);
    if (clickedNode && clickedNode.type === 'default') {
        console.log("폴더 클릭됨, 정책 세부 정보 갱신하지 않음.");
        return;
    }

    if (policy) {
        console.log("Policy details:", policy);

        var enabledChecked = policy.enabledValue === '1' ? 'checked' : '';
        var disabledChecked = policy.disabledValue === '0' ? 'checked' : '';
        var notConfiguredChecked = policy.enabledValue !== '1' && policy.disabledValue !== '0' ? 'checked' : '';

        var supportedOn = policy.supportedOn || 'Not specified';

        // 🔹 Elements 정보 추가
        var elementsHtml = '';
        if (policy.elements && policy.elements.length > 0) {
            elementsHtml = '<div><strong>Elements:</strong><ul>';
            policy.elements.forEach(el => {
                // Value Name을 ADML에서 찾아서 표시
                var valueNameDisplay = admlData.stringTable[el.valueName] || el.valueName;
                var defaultValue = '';

                // refId가 있을 경우 defaultValue를 ADML에서 찾음
                if (el.refId && admlData.stringTable[el.refId]) {
                    defaultValue = admlData.stringTable[el.refId];
                }

                elementsHtml += `<li><strong>ID:</strong> ${el.elementId || 'N/A'}<br>
                                 <strong>Type:</strong> ${el.elementType || 'N/A'}<br>
                                 <strong>Required:</strong> ${el.required || 'N/A'}`;

                // 🔹 각 elementType에 맞는 폼 요소 렌더링
                switch (el.elementType) {
                    case 'enum':
                        elementsHtml += '<br><strong>Enum Items:</strong><ul>';
                        el.items.forEach(item => {
                            elementsHtml += ` 
                                <li>
                                    <strong>Display Name:</strong> ${item.itemDisplayName}<br>
                                    <strong>Value:</strong> ${item.value}
                                </li>
                            `;
                        });
                        elementsHtml += '</ul>';
                        break;
                    case 'boolean':
                        elementsHtml += `
                            <label>
                                <input type="checkbox" ${el.required ? 'checked' : ''} />
                                ${el.valueName || 'Enable/Disable'}
                            </label>`;
                        break;
                    case 'string':
                        elementsHtml += `
                            <input type="text" value="${defaultValue || el.valueName || ''}" placeholder="Enter value here" />
                        `;
                        break;
                    case 'decimal':
                        elementsHtml += `
                            <input type="number" value="${el.valueName || ''}" step="0.01" />
                        `;
                        break;
                    case 'list':
                        elementsHtml += `
                            <br><strong>List Items:</strong><ul>
                        `;
                        el.items.forEach(item => {
                            var itemDisplayName = admlData.stringTable[item.displayName] || item.displayName;
                            elementsHtml += `
                                <li>${itemDisplayName}</li>
                            `;
                        });
                        elementsHtml += `</ul>`;
                        break;
                    default:
                        elementsHtml += `<p>Unknown element type: ${el.elementType}</p>`;
                }

                elementsHtml += ` 
                    <button onclick="showElementPopup('${el.elementId}', '${el.elementType}', '${el.required}', '${el.valueName}')">View Details</button>
                </li>`;
            });
            elementsHtml += '</ul></div>';
        } else {
            elementsHtml = '<div><strong>Elements:</strong> None</div>';
        }

        // 🔹 템플릿 리터럴로 전체 HTML 생성
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
            ${elementsHtml}  <!-- 🔹 Elements 표시 -->
        `;

        console.log("HTML to insert:", policyDetailsHtml);
        $('#policy-settings').html(policyDetailsHtml);
    } else {
        console.error("Policy not found for ID:", policyId);
        $('#policy-settings').html('<p>해당 정책을 찾을 수 없습니다.</p>');
    }
}


function showElementPopup(elementId, elementType, required, valueName) {
    var popupHtml = `
        <div id="elementPopup" style="position:fixed; top:20%; left:30%; width:40%; background-color:white; border:1px solid #ccc; padding:20px; z-index:1000;">
            <h3>Element Details</h3>
            <p><strong>ID:</strong> ${elementId}</p>
            <p><strong>Type:</strong> ${elementType}</p>
            <p><strong>Required:</strong> ${required}</p>
            <p><strong>Value Name:</strong> ${valueName}</p>
            <button onclick="closeElementPopup()">Close</button>
        </div>
    `;
    $('body').append(popupHtml);
}

function closeElementPopup() {
    $('#elementPopup').remove();
}

async function openProductBasePolicyPopup(){
    popupPolicyClear();
    const defaultPolicy = await loadDefaultPolicy();

    defaultPolicySetting(defaultPolicy, true);
	$('.productBasePolicyPopup').removeClass('hide');
}

function popupPolicyClear(){
    
    // policySelect  첫번쨰 값으로 설정
    $("#policySelect").val($("#policySelect option:first").val());
    var allPolicy = $(".polSection");
    $(allPolicy).addClass("hide");
    $(".fixedPol").removeClass("hide");

    // policyNameInput 초기화
    $("#policyNameInput").val('');
    $("#default70").val('');
    $("#default120").val('');
    $("#default121").val('');
    $("#default122").val('');
    $("#default127").val('');
    $("#default128").val('');
    $("#default309").val('');
    $("#default343").val('');
    $("#default345").val('');
    $("#default131").find('tbody').empty();
    $("#default499").find('tbody').empty();
    // 선택지 메뉴는 첫번째 값 선택
    $("#default2").val($("#default2 option:first").val());
    $("#default197").val($("#default197 option:first").val());
    $("#default392").val($("#default392 option:first").val());
    $("#default103").val($("#default103 option:first").val());
    $("#default105").val($("#default105 option:first").val());
    $("#default123").val($("#default123 option:first").val());
    $("#default124").val($("#default124 option:first").val());
    $("#default125").val($("#default125 option:first").val());
    $("#default126").val('');
    $("#default5120").val($("#default5120 option:first").val());
    $("#default302").val($("#default302 option:first").val());
    $("#default306").val($("#default306 option:first").val());
    $("#default307").val($("#default307 option:first").val());
    $("#default308").val($("#default308 option:first").val());
    
    // default132Checkbox 체크 해제
    $("#default132Checkbox").prop('checked', false);
    $("#default132").find('tbody').empty();
    // disabled 처리
    $("#default133").prop('disabled', true);
    $("#default134").prop('disabled', true);
    $("#default135").prop('disabled', true);
    $("#default133").val('');
    $("#default134").val('');
    $("#default135").val($("#default135 option:first").val());
    // default354Checkbox 체크 해제
    $("#default354Checkbox").prop('checked', false);
    $("#default354").find('tbody').empty();
    // default153 체크 해제
    $("#default153Checkbox").prop('checked', false);
    // default153 테이블 row 선택 해제
    $("#default153").find('tbody tr').each(function(){
        // disabled 처리
        $(this).find('td:first').find('input').prop('disabled', true);
        $(this).find('td:first').find('input').prop('checked', false);
    });
    
    // 저장버튼 보이기
    $('#saveBtn').removeClass('hide');

    // 수정버튼 숨기기
    $('#updateBtn').addClass('hide');

	$('.productBasePolicyPopup').addClass('hide');
}

// 테이블 행 생성을 처리하는 함수
function createTableRow(polId, item, isDefault) {
	const row = document.createElement('tr');
	
	// polId에 따라 다른 형식의 행 생성
	switch(polId) {
		case 131:
			row.innerHTML = `
				<td><input type="text" class="txtBox w100Per" value="${item.name}" disabled></td>
				<td><input type="text" class="txtBox w100Per_10" value="${item.val}" disabled></td>
			`;
			break;
        case 132:
            if(isDefault){
                return null;
            }
            else{
                row.innerHTML = `
                    <td><input type="text" class="txtBox w100Per" value="${item.name}" ></td>
                    <td><input type="text" class="txtBox w100Per_10" value="${item.val}" ></td>
                `;
            }
            break;
        case 354:
            if(isDefault){
                return null;
            }
            else{
                row.innerHTML = `
                    <td><input type="text" class="txtBox w100Per" value="${item.name}" ></td>
                    <td><input type="text" class="txtBox w100Per_10" value="${item.val}" ></td>
                `;
            }
            break;
		default:
			row.innerHTML = `
				<td><input type="text" class="txtBox w100Per" value="${item.name}" disabled></td>
				<td><input type="text" class="txtBox w100Per_10" value="${item.val}" disabled></td>
			`;
			break;
	}
	
	return row;
}

function loadMandatoryPolicy() {
    console.log('loadMandatoryPolicy');
    return new Promise((resolve, reject) => {
        $.ajax({
            url: '/primx/fun/pol/mandatory/list.do',
            type: 'POST',
            success: function(response) {
                if (response && response.resultList) {
                    // 원본 데이터 저장
                    originalPolicyData = response.resultList;
                    resolve(response.resultList);
                }
            },
            error: function(xhr, status, error) {
                console.error('정책 목록 로드 실패:', error);
                reject(error);
            }
        });
    });

}

function loadDefaultPolicy() {
    console.log('loadDefaultPolicy');
    return new Promise((resolve, reject) => {
        $.ajax({
            url: '/primx/fun/pol/default/list.do',
            type: 'POST',
            success: function(response) {
                if (response && response.resultList) {
                    // 원본 데이터 저장
                    defaultPolicyData = response.resultList;
                    resolve(response.resultList);
                }
            },
            error: function(xhr, status, error) {
                console.error('정책 목록 로드 실패:', error);
                reject(error);
            }
        });
    });

}

function customFlagSetting(polId, isActive){
    var elementId = 'default' + polId + 'Checkbox';
    var element = document.getElementById(elementId);
    console.log('elementId: ', elementId);
    console.log('element: ', element);
    // 해당 체크박스 체크
    element.checked = true;

    if(polId == 132){
        $("#default133").prop('disabled', isActive);
        $("#default134").prop('disabled', isActive);
        $("#default135").prop('disabled', isActive);
    }
    else if(polId == 354){
    }
    else if(polId == 153){
        $("#default153").find('tbody tr').each(function(){
            // disabled 처리
            $(this).find('td:first').find('input').prop('disabled', isActive);
        });
    }
}

function defaultPolicySetting(resultList, isDefault){
    
    console.log('resultList: ', resultList);
    resultList.forEach(function(item) {
        var elementId = 'default' + item.polId;
        var element = document.getElementById(elementId);

        if(!isDefault){
            if(item.polId == 132 || item.polId == 354 || item.polId == 153){
                customFlagSetting(item.polId, isDefault);
            }
        }

        if (element) {
            const parsedData = parsePolData(item.polData);
            if (parsedData) {
                console.log('elementId: ', elementId);
               switch(parsedData.type) {
                    
                    case 'int':
                        if (element.tagName === 'SELECT') {
                            element.value = parsedData.value;
                        }
                        break;

                    case 'bool':
                        if (element.tagName === 'SELECT') {
                            element.value = parsedData.value ? 'Y' : 'N';
                        }
                        // 체크박스의 경우
                        else if (element.tagName === 'INPUT' && element.type === 'checkbox') {
                            
                            // if(parsedData.value == 'true'){
                            //     console.log('--true-- ');
                            //     element.checked = true;
                            // }
                            // else{
                            //     console.log('--false-- ');
                            //     element.checked = false;
                            // }
                            element.checked = true;
                        }
                        break;
                    case 'sz':
                    case 'expandsz':
                        if (element.tagName === 'INPUT') {
                            element.value = parsedData.value;
                        }
                        break;
                    case 'list':
                        if (element.tagName === 'TABLE') {
                            const tbody = element.querySelector('tbody');
                            if (tbody) {
                                tbody.innerHTML = '';
                                parsedData.value.forEach(listItem => {
                                    const row = createTableRow(item.polId, listItem, isDefault);
                                    if(row != null){
                                        tbody.appendChild(row);
                                    }
                                });
                            }
                        }
                        break;
                }
            }
        }
    });
    
}

// XML 문자열에서 특수 문자를 제거하는 함수
function sanitizeXML(xmlString) {
	// 백슬래시를 임시로 다른 문자로 치환
	xmlString = xmlString.replace(/\\/g, '\\\\');
	
	// 제어 문자 제거 (ASCII 0-31, 127 제외)
	xmlString = xmlString.replace(/[\x00-\x08\x0B\x0C\x0E-\x1F\x7F]/g, '')
		// parsererror 태그와 그 내용 제거
		.replace(/<parsererror[\s\S]*?<\/parsererror>/g, '');
	
	// 임시로 치환한 백슬래시를 원래대로 복원
	return xmlString.replace(/\\\\/g, '\\');
}

// polData를 파싱하여 값을 추출하는 함수
function parsePolData(polData) {
	try {
		// XML 문자열 정제
		const cleanXml = sanitizeXML(polData);
		
		// XML 문자열을 파싱하기 위한 임시 div 생성
		const parser = new DOMParser();
		const xmlDoc = parser.parseFromString(cleanXml, 'text/xml');
		
		// parsererror 체크
		const parseError = xmlDoc.querySelector('parsererror');
		if (parseError) {
			console.error('XML 파싱 에러:', parseError);
			return null;
		}

		const policyElement = xmlDoc.querySelector('policy');
		if (!policyElement) return null;

		// 기본 구조 확인
		const result = {
			type: null,
			value: null
		};

        // int 타입 체크
        const intElement = policyElement.querySelector('int');
        if (intElement) {
            result.type = 'int';
            result.value = parseInt(intElement.textContent);
            return result;
        }

		// bool 타입 체크
		const boolElement = policyElement.querySelector('bool');
		if (boolElement) {
			result.type = 'bool';
			result.value = boolElement.textContent.toLowerCase() === 'true';
			return result;
		}

		// expandsz 타입 체크
		const expandszElement = policyElement.querySelector('expandsz');
		if (expandszElement) {
			result.type = 'expandsz';
			result.value = expandszElement.textContent;
			return result;
		}

		// sz 타입 체크
		const szElement = policyElement.querySelector('sz');
		if (szElement) {
			result.type = 'sz';
			result.value = szElement.textContent;
			return result;
		}

		// list 타입 체크
		const listElements = policyElement.querySelectorAll('list');
		if (listElements.length > 0) {
			result.type = 'list';
			result.value = Array.from(listElements).map(listElement => {
				const nameElement = listElement.querySelector('name');
				const valElement = listElement.querySelector('val');
				
				return {
					name: nameElement ? nameElement.textContent : '',
					val: valElement ? valElement.textContent : ''
				};
			});
			return result;
		}

		return null;
	} catch (error) {
		console.error('XML 파싱 중 에러 발생:', error);
		return null;
	}
}












function searchProductPolicy(){
	// val의 값으로 조건을 추가하고, 선택된 val의 텍스트를 필터 타이틀로 사용한다
	var searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']");
	var searchStartDate = $(".searchAreaPopup .search_wrap input[name='startDt']");
	var searchEndDate = $(".searchAreaPopup .search_wrap input[name='endDt']");
	var searchPolicyType = $(".searchAreaPopup .search_wrap select[name='searchPolicyType']");
	var searchCondition = $(".searchAreaPopup .search_wrap select[name='searchCondition']");
	var searchKeyword = $(".searchAreaPopup .search_wrap input[name='searchKeyword']");
	var searchProduct = $(".searchAreaPopup .search_wrap select[name='searchProduct']");

	if(searchDateType.val() != null && searchDateType.val() != "") {
		if(!searchFieldValid('date', searchStartDate.val(), searchEndDate.val(), searchDateType.val())) {
			return;
		}
		var filterStr = searchStartDate.val() +' ~ '+ searchEndDate.val();
		addFilterItem(".productPolicy .searchBox", "기간", filterStr, 'searchAreaPopup');
	}
	else {
        resetSearchValue("기간");
	}

	if(searchPolicyType.val() != null && searchPolicyType.val() != "") {
		addFilterItem(".productPolicy .searchBox", "유형", searchPolicyType.find('option:selected').text(), 'searchAreaPopup');
	}
	else {
        resetSearchValue("유형");
	}

	if(searchProduct.val() != null && searchProduct.val() != "") {
		addFilterItem(".productPolicy .searchBox", "제품", searchProduct.find('option:selected').text(), 'searchAreaPopup');
	}
	else {
        resetSearchValue("제품");
	}  

	if(searchCondition.val() != null && searchCondition.val() != "" && searchKeyword.val() != null && searchKeyword.val() != "") {
		if(!searchFieldValid('condition', searchKeyword.val(), null, null)) {
			return;
		}
		addFilterItem(".productPolicy .searchBox", "조건", searchCondition.find('option:selected').text() + " [" + searchKeyword.val() + "]", 'searchAreaPopup');
	}
	else {
        resetSearchValue("조건");   
	} 

    loadProductPolicyTable();

    $('.searchAreaPopup').addClass('hide');
}

function removeFilter(element, popupName) {

    if(popupName == 'searchAreaPopup') {
        // 선택된 필터의 filterTitle을 찾아서 초기화함수 호출
        var filterTitle = $(element).closest('.filterItem').find('.filterTitle').text();
        resetSearchValue(filterTitle);

        $(element).closest('.filterItem').remove(); // 해당 필터 아이템 제거
    }
    else {
        console.log('removeFilter: no match popupName: ', popupName);
    }
    
    loadProductPolicyTable();
}

function resetSearchValue(filterTitle) {
    console.log('resetSearchValue: ', filterTitle);

	var searchDateType = $(".searchAreaPopup .search_wrap select[name='dateSelect']");
	var searchStartDate = $(".searchAreaPopup .search_wrap input[name='startDt']");
	var searchEndDate = $(".searchAreaPopup .search_wrap input[name='endDt']");
	var searchPolicyType = $(".searchAreaPopup .search_wrap select[name='searchPolicyType']");
	var searchCondition = $(".searchAreaPopup .search_wrap select[name='searchCondition']");
	var searchKeyword = $(".searchAreaPopup .search_wrap input[name='searchKeyword']");
	var searchProduct = $(".searchAreaPopup .search_wrap select[name='searchProduct']");
    
    if(filterTitle == "기간") {
        searchDateType.val(searchDateType.find('option[value=""]').val());
        searchStartDate.val('');
        searchEndDate.val('');
    }   
    else if(filterTitle == "유형") {
        searchPolicyType.val(searchPolicyType.find('option[value=""]').val());
    }
    else if(filterTitle == "제품") {
        searchProduct.val(searchProduct.find('option[value=""]').val());
    }
    else if(filterTitle == "조건") {
        searchCondition.val(searchCondition.find('option[value="all"]').val());
        searchKeyword.val('');
    }
    
}

/**
 * bool 플래그 변경 함수
 * @param {Array} originalPolicyData - 원본 정책 데이터 배열
 * @param {Array} selectedPolicies - 선택된 정책 데이터 배열
 * @param {string} boolType - 변경할 bool 타입 (true 또는 false)
 * @returns {Array} 변경된 정책 데이터 배열
 */
function changeBoolFlag(originalPolicyData, selectedPolicies, boolType) {
    return originalPolicyData
        .filter(item => selectedPolicies.some(selectedPolicy => selectedPolicy.polId === item.polId))
        .map(item => {
            var copyItem = {...item}; // 깊은 복사
            if(copyItem.polData) {
                copyItem.polData = copyItem.polData.replace(
                    /<bool>\s*(false|False|FALSE|true|True|TRUE)\s*<\/bool>/i,
                    `<bool>${boolType}</bool>`
                );
            }
            console.log('copyItem: ', copyItem);
            return copyItem;
        });
}

function saveProductPolicy(){

    // productSelect 값 가져오기
    var productSelect = $("#productSelect").val();
    console.log('productSelect: ', productSelect);

    // policyNameInput 값 가져오기
    var policyNameInput = $("#policyNameInput").val();
    console.log('policyNameInput: ', policyNameInput);

    // policyNameInput 값이 없는 경우 알람 표시 및 종료
    if(policyNameInput == null || policyNameInput == ""){
        alert('정책명을 입력해주세요.');
        return;
    }

    // policyTypeInput 값 가져오기
    var policyTypeInput = $("#policyTypeInput").val();
    console.log('policyTypeInput: ', policyTypeInput);


    copyPolicyData = [];


    console.log('originalPolicyData: ', originalPolicyData);
    
    // 사용자 인터페이스(153) 정책 사용 시
    if($("#default153Checkbox").prop('checked')) {
        // 153 정책이 체크되었기 때문에 fixedPolicyIds에 153 추가
        const policy = loadFixedPolicy(originalPolicyData, 153);
        if (policy) {  // null이 아닌 경우만 추가
            copyPolicyData.push(policy);
        }
        else {
            return;
        }
        
        // 체크된 행 데이터 수집
        var checkedRows = $("#default153 tbody tr").filter(function() {
            return $(this).find('input[type="checkbox"]').prop('checked');
        });

        if (checkedRows.length === 0) {
            alert('선택된 정책이 없습니다.');
            return;
        }

        var selectedPolicies = [];
        checkedRows.each(function() {
            var row = $(this);
            var policy = {
                polId: parseInt(row.find('td:eq(1) input').val()),
                policy: row.find('td:eq(2) input').val()
            };
            selectedPolicies.push(policy);
            // console.log('Selected policy:', policy);
        });

        // console.log('Selected policies:', selectedPolicies);

        // bool 플래그 변경 함수 호출하여 결과를 바로 copyPolicyData에 할당
        const modifiedPolicies = changeBoolFlag(originalPolicyData, selectedPolicies, 'true');
        // console.log('modifiedPolicies: ', modifiedPolicies);
        copyPolicyData.push(...modifiedPolicies);

        // console.log('copyPolicyData: ', copyPolicyData);
    }
    else {
        console.log('default153Checkbox 체크되지 않음');
    }

    // 132 정책 사용 시
    if($("#default132Checkbox").prop('checked')) {

        for (let polId of safetyZonePolicyIds) {
             const policy = changePolicyData(originalPolicyData, polId);
             if (policy) {
                copyPolicyData.push(policy);
             }
             else {
                return;
             }
        }
        
        const policy = changeListPolicyData(originalPolicyData, 132, 'default132');
        if (policy) {
           copyPolicyData.push(policy);
        }
        else {
           return;
        }
    }

    // 354 정책 사용 시
    if($("#default354Checkbox").prop('checked')){
        // 354 정책이 체크되었기 때문에 fixedPolicyIds에 354 추가

        const policy = changeListPolicyData(originalPolicyData, 354, 'default354');
        if (policy) {
           copyPolicyData.push(policy);
        }
        else {
           return;
        }

    }
    
    // 고정 정책 로드
    for (let polId of fixedPolicyIds) {
        const policy = loadFixedPolicy(defaultPolicyData, polId);
        if (policy) {  // null이 아닌 경우만 추가
            copyPolicyData.push(policy);
        }
        else {
            return;
        }
    }


    console.log('total copyPolicyData: ', copyPolicyData);

    $.ajax({
        url: '/primx/fun/pol/product/saveProductPolicy.do',
        type: 'POST',
        contentType: 'application/json',  // JSON 형식으로 전송
        data: JSON.stringify({            // 데이터를 JSON 문자열로 변환
            productName: productSelect,
            policyName: policyNameInput,
            copyPolicyData: copyPolicyData
        }),
        success: function(data){
            console.log('data: ', data);
            if(data.status === "success") {
                // POL-2023050500001 형식으로 변환
                alert('정책 저장 성공');
                loadProductPolicyTable();
                $(".productBasePolicyPopup").addClass('hide');
            } else {
                alert('정책 저장 실패: ' + data.message);
            }
        },
        error: function(xhr, status, error){
            console.error('error: ', error);
            alert('정책 저장 중 오류가 발생했습니다.');
        }
    });
}


function updateProductPolicy(){

    // productSelect 값 가져오기
    var productSelect = $("#productSelect").val();
    console.log('productSelect: ', productSelect);

    // policyNameInput 값 가져오기
    var policyNameInput = $("#policyNameInput").val();
    console.log('policyNameInput: ', policyNameInput);

    // policyNameInput 값이 없는 경우 알람 표시 및 종료
    if(policyNameInput == null || policyNameInput == ""){
        alert('정책명을 입력해주세요.');
        return;
    }

    // policyCodeInput 값 가져오기
    var policyCodeInput = $('#policyCodeInput').val();
    console.log('policyCodeInput: ', policyCodeInput);

    // policyTypeInput 값 가져오기
    var policyTypeInput = $("#policyTypeInput").val();
    console.log('policyTypeInput: ', policyTypeInput);


    copyPolicyData = [];


    console.log('originalPolicyData: ', originalPolicyData);
    
    // 사용자 인터페이스(153) 정책 사용 시
    if($("#default153Checkbox").prop('checked')) {
        // 153 정책이 체크되었기 때문에 fixedPolicyIds에 153 추가
        const policy = loadFixedPolicy(originalPolicyData, 153);
        if (policy) {  // null이 아닌 경우만 추가
            copyPolicyData.push(policy);
        }
        else {
            return;
        }
        
        // 체크된 행 데이터 수집
        var checkedRows = $("#default153 tbody tr").filter(function() {
            return $(this).find('input[type="checkbox"]').prop('checked');
        });

        if (checkedRows.length === 0) {
            alert('선택된 정책이 없습니다.');
            return;
        }

        var selectedPolicies = [];
        checkedRows.each(function() {
            var row = $(this);
            console.log('row: ', row);
            console.log('row.find: ', row.find('td:eq(1) input').val());
            console.log('row.find: ', row.find('td:eq(2) input').val());
            var policy = {
                polId: parseInt(row.find('td:eq(1) input').val()),
                policy: row.find('td:eq(2) input').val()
            };
            selectedPolicies.push(policy);
            // console.log('Selected policy:', policy);
        });

        // console.log('Selected policies:', selectedPolicies);

        // bool 플래그 변경 함수 호출하여 결과를 바로 copyPolicyData에 할당
        const modifiedPolicies = changeBoolFlag(originalPolicyData, selectedPolicies, 'true');
        console.log('modifiedPolicies: ', modifiedPolicies);
        copyPolicyData.push(...modifiedPolicies);

        // console.log('copyPolicyData: ', copyPolicyData);
    }
    else {
        console.log('default153Checkbox 체크되지 않음');
    }

    // 132 정책 사용 시
    if($("#default132Checkbox").prop('checked')) {

        for (let polId of safetyZonePolicyIds) {
             const policy = changePolicyData(originalPolicyData, polId);
             if (policy) {
                copyPolicyData.push(policy);
             }
             else {
                return;
             }
        }
        
        const policy = changeListPolicyData(originalPolicyData, 132, 'default132');
        if (policy) {
           copyPolicyData.push(policy);
        }
        else {
           return;
        }
    }

    // 354 정책 사용 시
    if($("#default354Checkbox").prop('checked')){
        // 354 정책이 체크되었기 때문에 fixedPolicyIds에 354 추가

        const policy = changeListPolicyData(originalPolicyData, 354, 'default354');
        if (policy) {
           copyPolicyData.push(policy);
        }
        else {
           return;
        }

    }
    
    // 고정 정책 로드
    for (let polId of fixedPolicyIds) {
        const policy = loadFixedPolicy(defaultPolicyData, polId);
        if (policy) {  // null이 아닌 경우만 추가
            copyPolicyData.push(policy);
        }
        else {
            return;
        }
    }


    console.log('total copyPolicyData: ', copyPolicyData);

    $.ajax({
        url: '/primx/fun/pol/product/updateProductPolicy.do',
        type: 'POST',
        contentType: 'application/json',  // JSON 형식으로 전송
        data: JSON.stringify({            // 데이터를 JSON 문자열로 변환
            productName: productSelect,
            policyName: policyNameInput,
            policyCode: policyCodeInput,
            copyPolicyData: copyPolicyData
        }),
        success: function(data){
            console.log('data: ', data);
            if(data.status === "success") {
                // POL-2023050500001 형식으로 변환
                alert('정책 수정 성공');
                loadProductPolicyTable();
                $(".productBasePolicyPopup").addClass('hide');
            } else {
                alert('정책 수정 실패: ' + data.message);
            }
        },
        error: function(xhr, status, error){
            console.error('error: ', error);
            alert('정책 수정 중 오류가 발생했습니다.');
        }
    });
}

function loadFixedPolicy(defaultPolicyData, polId){
    // originalPolicyData에서 polId에 해당하는 데이터를 찾음
    var fixedPolicy = defaultPolicyData.find(item => item.polId === polId);
    
    if (!fixedPolicy) {
        alert(`정책 ID ${polId}에 해당하는 정책을 찾을 수 없습니다.`);
        return null;  // undefined 대신 null 반환
    }
    
    console.log('fixedPolicy: ', fixedPolicy);
    return fixedPolicy;
}

function changeListPolicyData(originalPolicyData, polId, tableId) {
    // originalPolicyData에서 polId에 해당하는 데이터를 찾음
    var fixedPolicy = originalPolicyData.find(item => item.polId === polId);
    
    if (!fixedPolicy) {
        alert(`정책 ID ${polId}에 해당하는 정책을 찾을 수 없습니다.`);
        return null;
    }

    try {
        // polData를 파싱
        const parser = new DOMParser();
        const xmlDoc = parser.parseFromString(fixedPolicy.polData, 'text/xml');
        
        // policy 엘리먼트 찾기
        const policyElement = xmlDoc.querySelector('policy');
        if (!policyElement) {
            alert('정책 데이터에서 policy 엘리먼트를 찾을 수 없습니다.');
            return null;
        }

        // 기존의 모든 list 엘리먼트 제거
        const existingLists = policyElement.querySelectorAll('list');
        existingLists.forEach(list => list.remove());

        // default354 테이블에서 데이터 가져오기
        const table = document.getElementById(tableId);
        const rows = table.querySelectorAll('tbody tr');

        console.log('rows: ', rows);
        
        // 각 행에 대해 새로운 list 엘리먼트 생성
        rows.forEach(row => {
            const nameInput = row.querySelector('td:first-child input');
            const valInput = row.querySelector('td:last-child input');
            console.log('nameInput: ', nameInput);
            console.log('valInput: ', valInput);
            
            if (nameInput && valInput && nameInput.value && valInput.value) {
                const listElement = xmlDoc.createElement('list');
                
                const nameElement = xmlDoc.createElement('name');
                nameElement.textContent = nameInput.value;
                
                const valElement = xmlDoc.createElement('val');
                valElement.textContent = valInput.value;
                
                listElement.appendChild(nameElement);
                listElement.appendChild(valElement);
                policyElement.appendChild(listElement);
            }
        });

        // 수정된 XML을 문자열로 변환하여 저장
        fixedPolicy.polData = xmlDoc.documentElement.outerHTML;
        
        console.log('수정된 fixedPolicy:', fixedPolicy);
        return fixedPolicy;
        
    } catch (error) {
        console.error('XML 파싱 중 에러 발생:', error);
        alert(`정책 ID ${polId}의 데이터 처리 중 오류가 발생했습니다.`);
        return null;
    }
}


function changePolicyData(originalPolicyData, polId) {
    // originalPolicyData에서 polId에 해당하는 데이터를 찾음
    var fixedPolicy = originalPolicyData.find(item => item.polId === polId);
    
    if (!fixedPolicy) {
        alert(`정책 ID ${polId}에 해당하는 정책을 찾을 수 없습니다.`);
        return null;
    }
    
    // default+polId 요소의 값을 가져옴
    const elementValue = $(`#default${polId}`).val();
    
    if (!elementValue) {
        if(emptyPolicyIds.includes(polId)){
            console.log('정책 ID ' + polId + '의 입력 값이 없습니다. (빈 값 허용 정책 알림)');
        }
        else{
            alert(`정책 ID ${polId}의 입력 값이 없습니다.`);
            return null;
        }
    }
    
    try {
        // polData를 파싱
        const parser = new DOMParser();
        const xmlDoc = parser.parseFromString(fixedPolicy.polData, 'text/xml');
        
        // <sz> 또는 <expandsz> 태그 찾기
        const szElement = xmlDoc.querySelector('sz');
        const expandszElement = xmlDoc.querySelector('expandsz');
        const intElement = xmlDoc.querySelector('int');
        
        if (szElement) {
            szElement.textContent = elementValue;
            fixedPolicy.polData = xmlDoc.documentElement.outerHTML;
        } else if (expandszElement) {
            expandszElement.textContent = elementValue;
            fixedPolicy.polData = xmlDoc.documentElement.outerHTML;
        } else if (intElement) {
            intElement.textContent = elementValue;
            fixedPolicy.polData = xmlDoc.documentElement.outerHTML;
        } else {
            alert(`정책 ID ${polId}에 sz 또는 expandsz 또는 int 태그가 없습니다.`);
            return null;
        }
        
        console.log('수정된 fixedPolicy: ', fixedPolicy);
        return fixedPolicy;
        
    } catch (error) {
        console.error('XML 파싱 중 에러 발생:', error);
        alert(`정책 ID ${polId}의 데이터 처리 중 오류가 발생했습니다.`);
        return null;
    }
}

function addRow(selector, id){
    var message = "";
    if(selector == "#default354Checkbox"){
        message = 'SafetyZone 접근 프로세스(354)가 체크되어 있지 않습니다.';
    }
    else if(selector == "#default132Checkbox"){
        message = 'SafetyZone 영역(132)가 체크되어 있지 않습니다.';
    }

    // default354Checkbox가 체크되어 있는 경우
    if($(selector).prop('checked')){
        // default354 테이블의 body에 row 추가
        var table = document.getElementById(id);
        var body = table.querySelector('tbody');
        var row = body.insertRow(body.rows.length);
        var cell1 = row.insertCell(0);
        var cell2 = row.insertCell(1);
        cell1.innerHTML = '<input type="text" name="" class="txtBox w100Per" value="">';
        cell2.innerHTML = '<input type="text" name="" class="txtBox w100Per_10" value="">';
    }
    else{
        alert(message);
    }

}

function deleteRow(selector, id){
    
    var message = "";
    if(selector == "#default354Checkbox"){
        message = 'SafetyZone 접근 프로세스(354)가 체크되어 있지 않습니다.';
    }
    else if(selector == "#default132Checkbox"){
        message = 'SafetyZone 영역(132)가 체크되어 있지 않습니다.';
    }

    // default354Checkbox가 체크되어 있는 경우
    if($(selector).prop('checked')){
        // default354 테이블의 body에 row가 없는 경우 알람 표시 및 종료
        var table = document.getElementById(id);
        var body = table.querySelector('tbody');
        if(body.rows.length == 0){
        alert('삭제할 행이 없습니다.');
            return;
        }
        
        // 테이블의 마지막 행을 삭제
        var row = body.rows[body.rows.length - 1];
        body.deleteRow(body.rows.length - 1);
    }
    else{
        alert(message);
    }
}

function getPolicyInfo(policyCode){
    return new Promise((resolve, reject) => {
        $.ajax({
            url: '/primx/fun/pol/product/getPolicyInfo.do',
            type: 'POST',
            data: {
                policyCode: policyCode
            },
            success: function(data){
                resolve({
                    policy: data.policy,
                    policyDetailList: data.policyDetailList
                });
            },
            error: function(xhr, status, error){
                console.error('error: ', error);
                reject(error);
            }
        });
    });
}

// copyPolicy 함수도 async/await를 사용하여 수정
async function copyPolicy(){
    popupPolicyClear();
    // productPolicyTable 테이블의 선택된 row 가져오기
    var selectedRow = $("#productPolicyTable tbody tr").filter(function() {
        return $(this).find('input[type="checkbox"]').prop('checked');
    });

    // 선택된 row가 0,1,n개인지 확인
    if(selectedRow.length == 0 || selectedRow.length > 1){
        alert('복사할 정책을 하나 선택해주세요.');
        return;
    }

    // console.log('selectedRow: ', selectedRow);

    // 선택된 row의 정보 모두 가져오기
    var selectedPolicyInfo = selectedRow.find('td');
    // console.log('selectedPolicyInfo: ', selectedPolicyInfo);

    // 정책 명 가져오기
    var policyName = selectedPolicyInfo.eq(5).text();
    console.log('policyName: ', policyName);
    var policyNameInput = $('#policyNameInput');
    policyNameInput.val(policyName+'_copy');

    // 정책 ID 가져오기
    var policyCode = selectedPolicyInfo.eq(4).text();
    console.log('policyCode: ', policyCode);

    try {
        await loadDefaultPolicy();

        const copyPolicy = await getPolicyInfo(policyCode);
        console.log('policyInfo: ', copyPolicy);
        
        defaultPolicySetting(copyPolicy.policyDetailList, false);
        $('.productBasePolicyPopup').removeClass('hide');
    } catch(error) {
        console.error('정책 정보 가져오기 실패:', error);
    }
}

function initDeletePolicy(){
    // productPolicyTable 테이블의 선택된 row 가져오기
    var selectedRow = $("#productPolicyTable tbody tr").filter(function() {
        return $(this).find('input[type="checkbox"]').prop('checked');
    });

    // 선택된 row가 0,1,n개인지 확인
    if(selectedRow.length == 0 || selectedRow.length > 1){
        alert('삭제할 정책을 하나 선택해주세요.');
        return;
    }

    $(".polDelPopup").removeClass('hide');
}

function deletePolicy(){
    // productPolicyTable 테이블의 선택된 row 가져오기
    var selectedRow = $("#productPolicyTable tbody tr").filter(function() {
        return $(this).find('input[type="checkbox"]').prop('checked');
    });

    console.log('selectedRow: ', selectedRow);

    // 선택된 row의 정보 모두 가져오기
    var selectedPolicyInfo = selectedRow.find('td');
    console.log('selectedPolicyInfo: ', selectedPolicyInfo);

    // 제품명 가져오기
    var productName = selectedPolicyInfo.eq(3).text();
    console.log('productName: ', productName);

    // 정책 ID 가져오기
    var policyCode = selectedPolicyInfo.eq(4).text();
    console.log('policyCode: ', policyCode);

    $.ajax({
        url: '/primx/fun/pol/product/deletePolicy.do',
        type: 'POST',
        data: {
            policyCode: policyCode,
            productName: productName
        },
        success: function(data){
            console.log('data: ', data);
            if(data.status === "success"){
                alert('정책 삭제 성공');
            } else {
                alert('정책 삭제 실패: ' + data.message);
            }
            $(".polDelPopup").addClass('hide');
            loadProductPolicyTable();
        },
        error: function(xhr, status, error){
            console.error('error: ', error);
        }
    });
}

function initDefaultPolicyChange(){
    
    // productPolicyTable 테이블의 선택된 row 가져오기
    var selectedRow = $("#productPolicyTable tbody tr").filter(function() {
        return $(this).find('input[type="checkbox"]').prop('checked');
    });

    // 선택된 row가 0,1,n개인지 확인
    if(selectedRow.length == 0 || selectedRow.length > 1){
        alert('기본 정책 적용할 정책을 하나 선택해주세요.');
        return;
    }

    $(".defaultChangePopup").removeClass('hide');
}

function applyDefaultPolicy(){
    
    // productPolicyTable 테이블의 선택된 row 가져오기
    var selectedRow = $("#productPolicyTable tbody tr").filter(function() {
        return $(this).find('input[type="checkbox"]').prop('checked');
    });

    console.log('selectedRow: ', selectedRow);

    // 선택된 row의 정보 모두 가져오기
    var selectedPolicyInfo = selectedRow.find('td');
    console.log('selectedPolicyInfo: ', selectedPolicyInfo);

    // 정책 ID 가져오기
    var policyCode = selectedPolicyInfo.eq(4).text();
    console.log('policyCode: ', policyCode);


    $.ajax({
        url: '/primx/fun/pol/product/applyDefaultPolicy.do',
        type: 'POST',
        data: {
            policyCode: policyCode
        },
        success: function(data){
            console.log('data: ', data);
            if(data.status === "success"){
                alert('기본 정책 적용 성공');
            } else {
                alert('기본 정책 적용 실패: ' + data.message);
            }
            $(".defaultChangePopup").addClass('hide');
            loadProductPolicyTable();
        },
        error: function(xhr, status, error){
            console.error('error: ', error);
        }
    });
    $(".defaultChangePopup").addClass('hide');
}

function openSystemStatusPopup(){

    // polSysStatusTable 테이블의 선택된 row 가져오기
    var selectedRow = $("#productPolicyTable tbody tr").filter(function() {
        return $(this).find('input[type="checkbox"]').prop('checked');
    });

    // 선택된 row가 0,1,n개인지 확인
    if(selectedRow.length == 0 || selectedRow.length > 1){
        alert('시스템 현황을 볼 정책을 하나 선택해주세요.');
        return;
    }
    // 선택된 row의 정보 모두 가져오기
    var selectedPolicyInfo = selectedRow.find('td');

    // 정책 ID 가져오기
    var policyCode = selectedPolicyInfo.eq(4).text();
    console.log('policyCode: ', policyCode);

    // polSysStatusPopupHeaderName 에 정책명 policyCode로 변경
    $('#polSysStatusPopupHeaderName').text(policyCode+' > ');

    // 정책 정보 가져오기
    loadSysStatusTable(policyCode);

    
    $('.polSysStatusPopup').removeClass('hide');
}

function searchSystem(){
    loadSysStatusTable($('#polSysStatusPopupPolicyCode').val());
}

function loadSysStatusTable(policyCode){
    // 기존 테이블이 있다면 제거
	if (polSysStatusTable) {
		polSysStatusTable.destroy();
		polSysStatusTable = null;
	}

	// 기존 CSV 버튼 이벤트 바인딩 제거
	$('#polSysStatusCsvExport').off('click');
    
    $('#polSysStatusPopupPolicyCode').val(policyCode);
	polSysStatusTable = initDataTables('#polSysStatusTable' ,{
		ajax: {
			url: '/primx/fun/sys/systemStatusPolicy/list.do',
			type: 'POST',
			data: function(data) {
				data.policyCode = policyCode;
                data.searchWord = $('#searchWord').val();
			},
			dataSrc: function (json) {
                console.log('json: ', json);
				// 일련번호 생성
				var data = json.data;
				for (var i = 0; i < data.length; i++) {
					var idx = json.recordsTotal - ((json.page-1) * json.pageSize) - i;
					data[i].rowId = idx;
				}
				return data;
			}
		},
		processing: true,
    	serverSide: true,
		columns: [
	        { data: null },
	        { data: 'rowId' },
	        { data: 'agentId' },
	        { data: 'computerName' },
	        {
				data: null,
				render: function (data, type, row, meta) {
					return getPolicyZone(row, 'ZC');
				}
			},
	        //{
			//	data: null,
			//	render: function (data, type, row, meta) {
			//		return getPolicyZone(row, 'OZ');
			//	}
			//},
	        {
				data: 'commonSystemYn',
				render: function (data, type, row, meta) {
					if (data == 'Y') {
						return '<span class="common" title="공용">공용</span>';
					} else {
						return '<span class="user" title="사용자">사용자</span>';
					}
				},
				createdCell: function (td, cellData, rowData, row, col) {
					$(td).addClass('system-type');
				}
	        },
	        { data: 'userNm' },
	        { data: 'deptName' }
	        //{ data: 'winVersionString' },
	        //{ data: 'agentVersion' }
	        // { data: 'policyName' },
	        // {
			// 	data: 'insertDate',
			// 	render: function(data, type, row, meta) {
			// 		return moment(data).format('YYYY-MM-DD');
			// 	}
			// },
	        // {
			// 	data: 'lastKaDate',
			// 	render: function(data, type, row, meta) {
			// 		return moment(data).format('YYYY-MM-DD HH:mm:ss');
			// 	}
			// }
	    ],
		columnDefs: [
			{
				render: DataTable.render.select(),
				targets: 0
			}
		],
		buttons: [
			{
				extend: 'csv',
				title: `${firstDepthTit}_${secondDepthTit}_${formattedDate}`,
				exportOptions: {
					columns: ':not(:first-child)'  // 첫 번째 컬럼(체크박스)을 제외한 모든 컬럼 내보내기
				},
				bom: true, // UTF-8 BOM 추가
				customize: function(csv) {
					return '\ufeff' + csv; // UTF-8 BOM 추가하여 한글 깨짐 방지
				}
			}
		],
		select: {
			style: 'multi',
			selector: 'td:first-child'
		}
	});

    // CSV 내보내기 버튼과 연결
    $('#polSysStatusCsvExport').on('click', function() {
        polSysStatusTable.button('.buttons-csv').trigger();
    });
	
}

/**
 * policy ZC, OZ 체크
 */
function getPolicyZone(row, type) {
	var policys = row.policys;
	var rtnStr = '';
	if (policys) {
		if (policys.includes(',')) {
			var policyArr = policys.split(',');
			for (var i = 0; i < policyArr.length; i++) {
				var productId = policyArr[i].split('||')[2];
				if (type == 'ZC' && productId == 1) {
					rtnStr = '<span class="checkIco" title="'+ policyArr[i].split('||')[1] +'">'+ policyArr[i].split('||')[1] +'</span>';
					break;
				} else if (type == 'OZ' && productId == 2) {
					rtnStr = '<span class="checkIco" title="'+ policyArr[i].split('||')[1] +'">'+ policyArr[i].split('||')[1] +'</span>';
					break;
				}
			}
		} else {
			var productId = policys.split('||')[2];
			if (type == 'ZC' && productId == 1) {
				rtnStr = '<span class="checkIco" title="'+ policys.split('||')[1] +'">'+ policys.split('||')[1] +'</span>';
			} else if (type == 'OZ' && productId == 2) {
				rtnStr = '<span class="checkIco" title="'+ policys.split('||')[1] +'">'+ policys.split('||')[1] +'</span>';
			}
		}
	}
	return rtnStr;
}

function initDeleteSystem(){
    $('.polSysDelPopup').removeClass('hide');
}

function deleteSystem(){
    
    // polSysStatusTable 테이블의 선택된 row 가져오기
    var selectedRow = $("#polSysStatusTable tbody tr").filter(function() {
        return $(this).find('input[type="checkbox"]').prop('checked');
    });

    // 선택된 row가 0,1,n개인지 확인
    if(selectedRow.length == 0){
        alert('시스템 제거할 시스템을 하나 이상 선택해주세요.');
        return;
    }

    // 선택된 row의 정보 모두 가져오기
    var selectedSystemInfoList = [];
    selectedRow.each(function() {
        var selectedSystemInfo = $(this).find('td');
        selectedSystemInfoList.push(selectedSystemInfo);
    });
    console.log('selectedSystemInfoList: ', selectedSystemInfoList);

    // 시스템 ID 가져오기
    var agentIdList = [];
    selectedSystemInfoList.forEach(function(systemInfo) {
        var agentId = $(systemInfo[2]).text();
        agentIdList.push(agentId);
    });
    console.log('agentIdList: ', agentIdList);
    
    $.ajax({
        url: '/primx/fun/pol/product/deleteSystem.do',
        type: 'POST',
        traditional: true,  // 배열 파라미터 전송을 위해 필요
        data: {
            agentIdList: agentIdList
        },
        success: function(data){
            console.log('data: ', data);
            if(data.status === "success"){
                alert('시스템 제거 성공');
            } else {
                alert('시스템 제거 실패: ' + data.message);
            }
            $('.polSysDelPopup').addClass('hide');
            loadSysStatusTable($('#polSysStatusPopupPolicyCode').val());
            loadProductPolicyTable();
        },
        error: function(xhr, status, error){
            console.error('error: ', error);
        }
    });

    $('.polSysDelPopup').addClass('hide');
}

function initAddSystem(){

    // 정책 정보 가져오기
    loadAddSysStatusTable(null);

    $('.polSysStatusAddPopup').removeClass('hide');
}

function searchAddSystem(){
    loadAddSysStatusTable(null);
}

function loadAddSysStatusTable(policyCode){

	polSysStatusAddPopupTable = initDataTables('#polSysStatusAddPopupTable' ,{
		ajax: {
			url: '/primx/fun/sys/systemStatusPolicy/list.do',
			type: 'POST',
			data: function(data) {
				data.policyCode = policyCode;
                data.searchWord = $('#searchWordAddSystem').val();
			},
			dataSrc: function (json) {
                console.log('json: ', json);
				// 일련번호 생성
				var data = json.data;
				for (var i = 0; i < data.length; i++) {
					var idx = json.recordsTotal - ((json.page-1) * json.pageSize) - i;
					data[i].rowId = idx;
				}
				return data;
			}
		},
		processing: true,
    	serverSide: true,
		columns: [
	        { data: null },
	        { data: 'rowId' },
	        { data: 'agentId' },
	        { data: 'computerName' },
	        {
				data: null,
				render: function (data, type, row, meta) {
					return getPolicyZone(row, 'ZC');
				}
			},
	        /*{
				data: null,
				render: function (data, type, row, meta) {
					return getPolicyZone(row, 'OZ');
				}
			},*/
	        {
				data: 'commonSystemYn',
				render: function (data, type, row, meta) {
					if (data == 'Y') {
						return '<span class="common" title="공용"></span>';
					} else {
						return '<span class="user" title="사용자"></span>';
					}
				},
				createdCell: function (td, cellData, rowData, row, col) {
					$(td).addClass('system-type');
				}
	        },
	        { data: 'userNm' },
	        { data: 'deptName' }
	        /*{ data: 'winVersionString' },
	        { data: 'agentVersion' }*/
	        // { data: 'policyName' },
	        // {
			// 	data: 'insertDate',
			// 	render: function(data, type, row, meta) {
			// 		return moment(data).format('YYYY-MM-DD');
			// 	}
			// },
	        // {
			// 	data: 'lastKaDate',
			// 	render: function(data, type, row, meta) {
			// 		return moment(data).format('YYYY-MM-DD HH:mm:ss');
			// 	}
			// }
	    ],
		columnDefs: [
			{
				render: DataTable.render.select(),
				targets: 0
			}
		],
		select: {
			style: 'multi',
			selector: 'td:first-child'
		}
	});

    // draw 이벤트 핸들러 추가
    polSysStatusAddPopupTable.on('draw', function () {
        var currentPolicyCode = $('#polSysStatusPopupPolicyCode').val(); // 현재 정책 코드
        
        // 테이블의 모든 행을 순회
        polSysStatusAddPopupTable.rows().every(function(rowIdx) {
            var rowData = this.data();
            var row = $(this.node()); // 현재 행의 DOM 요소
            
            // rowData의 policyCode가 현재 정책 코드와 같으면 disTr 클래스 추가
            if (rowData && rowData.policyCode === currentPolicyCode) {
                row.addClass('disTr');
            }
        });
    });
}

function addSystem(){
    // 선택된 시스템 정보 가져오기
    var selectedRow = $("#polSysStatusAddPopupTable tbody tr").filter(function() {
        return $(this).find('input[type="checkbox"]').prop('checked');
    });
    // 여러개 선택되어 있는지 확인
    if(selectedRow.length == 0){
        alert('시스템 추가할 시스템을 하나 이상 선택해주세요.');
        return;
    }

    // 선택된 여러 시스템 정보 가져오기
    var selectedSystemInfoList = [];
    selectedRow.each(function() {
        var selectedSystemInfo = $(this).find('td');
        selectedSystemInfoList.push(selectedSystemInfo);
    });
    console.log('selectedSystemInfoList: ', selectedSystemInfoList);

    // 시스템 ID 가져오기
    var agentIdList = [];
    selectedSystemInfoList.forEach(function(systemInfo) {
        // 시스템 ID는 세 번째 열(인덱스 2)에 있음
        var agentId = $(systemInfo[2]).text();
        agentIdList.push(agentId);
    });
    console.log('agentIdList: ', agentIdList);

    // 정책 코드 가져오기
    var policyCode = $('#polSysStatusPopupPolicyCode').val();
    console.log('policyCode: ', policyCode);
    
    $.ajax({
        url: '/primx/fun/pol/product/addSystem.do',
        type: 'POST',
        traditional: true,  // 배열 파라미터 전송을 위해 필요
        data: {
            agentIdList: agentIdList,
            policyCode: policyCode
        },
        success: function(data){
            console.log('data: ', data);
            if(data.status === "success"){
                alert('시스템 추가 성공');
                loadSysStatusTable($('#polSysStatusPopupPolicyCode').val());
                loadProductPolicyTable();
                $('.polSysStatusAddPopup').addClass('hide');
            } else {
                alert('시스템 추가 실패: ' + data.message);
            }
        },
        error: function(xhr, status, error){
            console.error('error: ', error);
        }
    });
}
