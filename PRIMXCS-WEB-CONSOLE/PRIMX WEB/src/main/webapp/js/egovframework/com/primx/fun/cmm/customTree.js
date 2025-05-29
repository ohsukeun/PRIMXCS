var CustomTree = (function() {
    function CustomTree(container, options) {
        this.container = container;
        this.options = {
            onSelect: options.onSelect || function() {},
            onDoubleClick: options.onDoubleClick || function() {},
            expandAll: options.expandAll !== undefined ? options.expandAll : true
        };
        this.lastClickTime = 0; 
    }

    CustomTree.prototype.render = function(data) {
        var tree = this.buildTreeStructure(data);
        console.log("tree: ", tree);
        // 기존 이벤트 리스너 제거
        if (this.container) {
            this.container.innerHTML = '';
            // 이벤트 리스너 복사본 저장
            var oldContainer = this.container.cloneNode(true);
            this.container.parentNode.replaceChild(oldContainer, this.container);
            this.container = oldContainer;
        }
        this.container.innerHTML = this.generateTreeHTML(tree);
        this.attachEventListeners();
    };

    CustomTree.prototype.buildTreeStructure = function(data) {
        var map = {};
        var roots = [];

        // 모든 노드를 맵으로 변환
        for (var i = 0; i < data.length; i++) {
            var item = data[i];
            map[item.deptId] = {
                deptId: item.deptId,
                deptName: item.deptName,
                parentId: item.parentId,
                level: item.level,
                orderIndex: item.orderIndex,
                companyId: item.companyId,
                insertedByHrYn: item.insertedByHrYn,
                children: []
            };
        }

        // 부모-자식 관계 구성
        for (var i = 0; i < data.length; i++) {
            var item = data[i];
            if (item.parentId && map[item.parentId]) {
                map[item.parentId].children.push(map[item.deptId]);
            } else {
                roots.push(map[item.deptId]);
            }
        }

        // level 기준으로 정렬
        roots.sort(function(a, b) {
            if (a.level !== b.level) {
                return a.level - b.level;
            }
            // level이 같으면 orderIndex로 정렬
            return (a.orderIndex || 0) - (b.orderIndex || 0);
        });

        return roots;
    };

    CustomTree.prototype.generateTreeHTML = function(nodes, level) {
        level = level || 0;
        if (!nodes.length) return '';

        var html = level === 0 ? 
            '<ul class="custom-tree">' : 
            '<ul class="tree-children" style="display: none;">'; // 항상 기본값은 숨김

        nodes.sort(function(a, b) {
            return (a.orderIndex || 0) - (b.orderIndex || 0);
        });

        for (var i = 0; i < nodes.length; i++) {
            var node = nodes[i];
            var hasChildren = node.children && node.children.length > 0;
            var isExpanded = this.options.searchMode || this.options.expandAll;
            var hrClass = node.insertedByHrYn === 'Y' ? 'hry' : 'hrn';
            
            html += '<li class="tree-node" data-id="' + node.deptId + 
                   '" data-name="' + node.deptName + 
                   '" data-company-id="' + node.companyId + '">' +
                   '<div class="tree-content">';
            
            if (hasChildren) {
                html += '<span class="toggle-btn' + (isExpanded ? ' expanded' : '') + '">' + '</span>';
            } else {
                html += '<span class="toggle-btn-placeholder"></span>';
            }
            
            html += '<span class="tree-label ' + hrClass + '">' + node.deptName + '</span>' +
                   '</div>';
            
            if (hasChildren) {
                var childrenHtml = this.generateTreeHTML(node.children, level + 1);
                if (isExpanded) {
                    // 검색 모드나 expandAll일 때는 하위 메뉴를 보이게 함
                    childrenHtml = childrenHtml.replace('style="display: none;"', 'style="display: block;"');
                }
                html += childrenHtml;
            }
            
            html += '</li>';
        }

        html += '</ul>';
        return html;
    };

    CustomTree.prototype.attachEventListeners = function() {
        var self = this;
        var clickDelay = 300; // 더블클릭 판단 시간 (밀리초)
        
        // 클릭 이벤트 처리
        this.container.addEventListener('click', function(e) {
            var toggleBtn = e.target.closest('.toggle-btn');
            var label = e.target.closest('.tree-label');
            
            if (toggleBtn) {
                // 토글 버튼 클릭 처리
                var node = toggleBtn.closest('.tree-node');
                var children = node.querySelector('.tree-children');
                if (children) {
                    var isExpanded = toggleBtn.classList.toggle('expanded');
                    children.style.display = isExpanded ? 'block' : 'none';
                    //toggleBtn.textContent = isExpanded ? '−' : '+';
                }
            } else if (label) {
                var currentTime = new Date().getTime();
                var timeDiff = currentTime - self.lastClickTime; // this.lastClickTime 대신 self.lastClickTime 사용
                // console.log('lastClickTime:'+self.lastClickTime);
                // console.log('currentTime:'+currentTime);
                // console.log('timeDiff:'+timeDiff);
                // if (timeDiff < clickDelay) {
                //     // 더블클릭으로 처리
                //     // console.log('더블클릭 감지');
                //     var node = label.closest('.tree-node');
                //     // console.log('더블클릭 감지 data-name:'+node.getAttribute('data-name'));
                //     // console.log('더블클릭 감지 data-id:'+node.getAttribute('data-id'));
                //     // console.log('더블클릭 감지 data-company-id:'+node.getAttribute('data-company-id'));
                //     if (self.options.onDoubleClick) {
                //         self.options.onDoubleClick(
                //             node.getAttribute('data-name'),
                //             node.getAttribute('data-id'),
                //             node.getAttribute('data-company-id')
                //         );
                //     }
                // } else {
                    // 단일 클릭으로 처리
                    // console.log('단일 클릭 감지');
                    var node = label.closest('.tree-node');
                    
                    // 기존 선택된 항목의 선택 해제
                    var selected = self.container.querySelectorAll('.tree-label.selected');
                    selected.forEach(function(el) {
                        el.classList.remove('selected');
                    });
                    
                    // 새로운 항목 선택
                    label.classList.add('selected');
                    
                    // onSelect 콜백 호출
                    if (self.options.onSelect) {
                        self.options.onSelect(
                            node.getAttribute('data-name'),
                            node.getAttribute('data-id'),
                            node.getAttribute('data-company-id')
                        );
                    }
                // }
                
                self.lastClickTime = currentTime; // 클래스 인스턴스 변수 업데이트
            }
        });

        // dblclick 이벤트 리스너 제거
    };

    CustomTree.prototype.searchNodes = function(keyword) {
        if (!keyword) {
            // 검색어가 없으면 모든 노드 표시
            this.container.querySelectorAll('.tree-node').forEach(node => {
                node.style.display = '';
            });
            return;
        }

        // 대소문자 구분 없이 검색
        keyword = keyword.toLowerCase();
        
        // 모든 노드를 순회하면서 검색어와 매칭되는지 확인
        this.container.querySelectorAll('.tree-node').forEach(node => {
            const deptName = node.getAttribute('data-name').toLowerCase();
            if (deptName.includes(keyword)) {
                // 검색어와 일치하는 노드는 표시
                node.style.display = '';
                // 부모 노드들도 모두 표시
                let parent = node.parentElement;
                while (parent) {
                    if (parent.classList.contains('tree-children')) {
                        parent.style.display = 'block';
                        const parentNode = parent.parentElement;
                        if (parentNode) {
                            const toggleBtn = parentNode.querySelector('.toggle-btn');
                            if (toggleBtn) {
                                toggleBtn.textContent = '−';
                                toggleBtn.classList.add('expanded');
                            }
                        }
                    }
                    parent = parent.parentElement;
                }
            } else {
                // 검색어와 일치하지 않는 노드는 숨김
                node.style.display = 'none';
            }
        });
    };

    return CustomTree;
})(); 