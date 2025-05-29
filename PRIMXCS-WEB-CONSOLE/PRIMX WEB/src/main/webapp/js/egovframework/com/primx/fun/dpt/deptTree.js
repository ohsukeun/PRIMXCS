console.log('DeptTree.js loading...', imgpath);

/**
 * 부서 트리 구현을 위한 JavaScript
 */

var DeptTree = {
    // 트리 아이콘 정보
    icons: {
        plus: imgpath + 'menu_plus.gif',
        plusBottom: imgpath + 'menu_plusbottom.gif',
        minus: imgpath + 'menu_minus.gif',
        minusBottom: imgpath + 'menu_minusbottom.gif',
        folder: imgpath + 'menu_folder.gif',
        folderOpen: imgpath + 'menu_folderopen.gif',
        page: imgpath + 'menu_page.gif',
        join: imgpath + 'menu_join.gif',
        joinBottom: imgpath + 'menu_joinbottom.gif',
        line: imgpath + 'menu_line.gif',
        empty: imgpath + 'menu_empty.gif',
        base: imgpath + 'menu_base.gif'
    },

    // 트리 생성
    createTree: function(container, treeData, options) {
        options = options || {};
        var defaultOptions = {
            expandAll: false,
            onSelect: null
        };
        var settings = this._extend({}, defaultOptions, options);
        console.log('DeptTree.createTree called with:', {
            container: container,
            treeData: treeData,
            options: settings
        });

        // 트리 데이터 구조화
        const structuredData = this._structureData(treeData);
        console.log('Structured tree data:', structuredData);
        
        // 트리 HTML 생성
        const treeHtml = this._generateTreeHtml(structuredData, settings);
        console.log('Generated tree HTML:', treeHtml);
        
        container.innerHTML = treeHtml;
        
        // 이벤트 핸들러 등록
        this._bindEvents(container, settings);
    },

    // 트리 데이터 구조화
    _structureData: function(flatData) {
        const dataMap = {};
        const rootItems = [];

        // 먼저 모든 아이템을 맵으로 변환
        flatData.forEach(item => {
            dataMap[item.deptId] = {
                ...item,
                children: []
            };
        });

        // 부모-자식 관계 구성
        flatData.forEach(item => {
            if (item.parentId && dataMap[item.parentId]) {
                dataMap[item.parentId].children.push(dataMap[item.deptId]);
            } else {
                rootItems.push(dataMap[item.deptId]);
            }
        });

        return rootItems;
    },

    // 트리 HTML 생성
    _generateTreeHtml: function(items, settings, level = 0) {
        let html = level === 0 ? '<div class="tree-container">' : '';
        
        items.forEach((item, index) => {
            const isLast = index === items.length - 1;
            const hasChildren = item.children && item.children.length > 0;
            const expanded = settings.expandAll;
            
            html += `<div class="tree-item" data-dept-id="${item.deptId}">`;
            
            // 들여쓰기와 연결선
            for (let i = 0; i < level; i++) {
                html += `<img src="${this.icons.line}" class="tree-line">`;
            }
            
            // 확장/축소 아이콘
            if (hasChildren) {
                const toggleIcon = expanded ? 
                    (isLast ? this.icons.minusBottom : this.icons.minus) :
                    (isLast ? this.icons.plusBottom : this.icons.plus);
                html += `<img src="${toggleIcon}" class="tree-toggle">`;
            } else {
                html += `<img src="${isLast ? this.icons.joinBottom : this.icons.join}" class="tree-join">`;
            }
            
            // 폴더/파일 아이콘과 이름
            const folderIcon = hasChildren ? 
                (expanded ? this.icons.folderOpen : this.icons.folder) : 
                this.icons.page;
            html += `
                <img src="${folderIcon}" class="tree-icon">
                <span class="tree-label">${item.deptName}</span>
            `;
            
            // 하위 항목 처리
            if (hasChildren) {
                html += `<div class="tree-children" ${!expanded ? 'style="display:none;"' : ''}>`;
                html += this._generateTreeHtml(item.children, settings, level + 1);
                html += '</div>';
            }
            
            html += '</div>';
        });
        
        if (level === 0) html += '</div>';
        return html;
    },

    // 이벤트 핸들러 등록
    _bindEvents: function(container, settings) {
        container.addEventListener('click', (e) => {
            const target = e.target;
            
            // 토글 아이콘 클릭
            if (target.classList.contains('tree-toggle')) {
                const item = target.closest('.tree-item');
                const children = item.querySelector('.tree-children');
                const isExpanded = children.style.display !== 'none';
                
                children.style.display = isExpanded ? 'none' : 'block';
                this._updateIcons(item, !isExpanded);
            }
            
            // 라벨 클릭
            if (target.classList.contains('tree-label')) {
                const item = target.closest('.tree-item');
                const deptId = item.dataset.deptId;
                
                // 선택 스타일 처리
                container.querySelectorAll('.tree-label.selected').forEach(el => {
                    el.classList.remove('selected');
                });
                target.classList.add('selected');
                
                // 선택 이벤트 콜백
                if (settings.onSelect) {
                    settings.onSelect(deptId);
                }
            }
        });
    },

    // 아이콘 업데이트
    _updateIcons: function(item, isExpanded) {
        const toggle = item.querySelector('.tree-toggle');
        const icon = item.querySelector('.tree-icon');
        const isLast = !item.nextElementSibling;
        
        toggle.src = isExpanded ?
            (isLast ? this.icons.minusBottom : this.icons.minus) :
            (isLast ? this.icons.plusBottom : this.icons.plus);
            
        icon.src = isExpanded ? this.icons.folderOpen : this.icons.folder;
    },

    // 객체 확장 헬퍼 함수 추가
    _extend: function() {
        var extended = {};
        for(var i = 0; i < arguments.length; i++) {
            var obj = arguments[i];
            for(var prop in obj) {
                if(obj.hasOwnProperty(prop)) {
                    extended[prop] = obj[prop];
                }
            }
        }
        return extended;
    }
}; 

// 전역 객체로 노출되었는지 확인
console.log('DeptTree object created:', DeptTree); 