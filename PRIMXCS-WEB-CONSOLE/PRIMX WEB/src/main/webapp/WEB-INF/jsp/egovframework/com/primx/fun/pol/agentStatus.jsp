<%@ page pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<title data-i18n="webTitle.agentPolicy">PRIMX | Agent정책</title>
<form class="agentStatus" action="">
	<div class="stickyWrap">
		<div class="titleWrap">
			<h2 class="title" data-i18n="navigation.agentPolicy"></h2>
			<h2 class="depthTitle">
				<span class="fir" data-i18n="navigation.policy"></span>
				<span class="fir"> > </span>
				<span class="sec" data-i18n="navigation.agentPolicy"></span>
			</h2>
		</div>
		<div class="searchInnerWrap">
			<div class="searchBox">
				<input type="text" class="txtBox searchInput" readonly data-i18n-placeholder="search.searchTit">
			</div>
			<div class="dropMenu">
				<p class="selectMenu" data-i18n="menu.menu">메뉴</p>
				<ul>
					<li class="agnAddBtn"><button type="button" onclick="openAddPopup();" data-i18n="menu.register">등록</button></li>
					<li class="agnCopyBtn"><button type="button" onclick="copyAgentPolicy();" data-i18n="menu.copyPolicy">정책 복사</button></li>
					<li class="agnDelBtn borbot"><button type="button" onclick="deleteAgentPolicy();" data-i18n="menu.delete">삭제</button></li>
					<li class="defaultChangeBtn borbot"><button type="button" onclick="initDefaultAgentPolicyChange();" data-i18n="menu.applyDefaultPolicy">기본 정책 적용</button></li>
					<li class="sysStatusBtn borbot"><button type="button" onclick="openSystemStatusPopup()" data-i18n="menu.systemStatus">시스템 현황</button></li>
					<li class="csv"><button type="button" id="agentStatusCsvExport" data-i18n="menu.csv">내보내기(CSV)</button></li>
				</ul>
			</div>
		</div>
	</div>
	<div class="tableWrap">
		<table class="dataTable checkDataTable" id="agentStatusTable">
			<thead>
				<tr>
					<th width="3%" data-i18n="agentPolicy.grid.select">선택</th>
					<th width="3%" data-i18n="agentPolicy.grid.number">번호</th>
					<th width="5%" data-i18n="agentPolicy.grid.polType">유형</th>
					<th width="" data-i18n="agentPolicy.grid.policyID">정책 ID</th>
					<th width="10%" data-i18n="agentPolicy.grid.policyName">정책명</th>
					<th width="5%" data-i18n="agentPolicy.grid.apply">적용</th>
					<th width="5%" data-i18n="agentPolicy.grid.registrant">등록자</th>
					<th width="5%" data-i18n="agentPolicy.grid.registrationDate">등록일</th>
					<th width="7%" data-i18n="agentPolicy.grid.modificationDate">수정일</th>
				</tr>
			</thead>
			<tbody>
				<!-- <tr>
					<td></td>
					<td>1</td>
					<td></td>
					<td>AGN-2024102100002</td>
					<td>기본 정책</td>
					<td>2</td>
					<td>admin 관리자</td>
					<td>2024-08-21</td>
					<td>2024-09-21</td>
				</tr>
				<tr>
					<td></td>
					<td>1</td>
					<td></td>
					<td>AGN-2024102100001</td>
					<td>개별 정책</td>
					<td>4</td>
					<td>admin 관리자</td>
					<td>2024-08-21</td>
					<td>2024-09-21</td>
				</tr> -->
			</tbody>
		</table>
	</div>
</form>

<!-- 테이블 정보&등록&복사&수정 팝업 -->
<section class="popup agentStatusViewPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="agentPolicy.agentStatusViewPopup.popTitle">정보</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="setWrap hide">
					<li class="line w2">
						<div class="item">
							<em data-i18n="agentPolicy.agentStatusViewPopup.registrant">등록자</em>
							<div class="" id="agentStatusViewPopupInsertedBy"><!-- admin 관리자 --></div>
						</div>
						<div class="item">
							<em data-i18n="agentPolicy.agentStatusViewPopup.registrationDate">등록일</em>
							<div class="" id="agentStatusViewPopupInsertDate"><!-- 2024-10-21 --></div>
						</div>
					</li>
					<li class="line w2">
						<div class="item">
							<em data-i18n="agentPolicy.agentStatusViewPopup.modifier">수정자</em>
							<div class="" id="agentStatusViewPopupUpdateBy"><!-- admin 관리자 --></div>
						</div>
						<div class="item">
							<em data-i18n="agentPolicy.agentStatusViewPopup.modificationDate">수정일</em>
							<div class="" id="agentStatusViewPopupUpdateDate"><!-- 2024-12-12 --></div>
						</div>
					</li>
				</ul>
				<ul class="setWrap">
					<li class="line" id="policyInfo">
						<div class="item">
							<em data-i18n="agentPolicy.agentStatusViewPopup.policyID">정책 ID</em>
							<div class="">
								<input type="text" class="txtBox policyId" value="" id="agentStatusViewPopupPolicyCode" readonly>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="agentPolicy.agentStatusViewPopup.policyName">정책명</em>
							<div class="">
								<input type="text" class="txtBox policyName" id="agentStatusViewPopupPolicyName" value="">
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<em data-i18n="agentPolicy.agentStatusViewPopup.policyType">정책 유형</em>
							<div class="" id="agentStatusViewPopupDefaultPolicyYn"><!-- 기본 --></div>
						</div>
					</li>
				</ul>
				<div class="edittabWrap">
					<ul class="tabTitle">
						<li class="on" data-i18n="agentPolicy.agentStatusViewPopup.trayMenu">트레이 메뉴</li>
						<li data-i18n="agentPolicy.agentStatusViewPopup.fileTransferZone">파일전송 Zone</li>
					</ul>
					<ul class="tabCont">
						<li class="on">
							<ul class="setWrap">
								<li class="line">
									<div class="item">
										<em class="em100Per"><input type="checkbox" name="" id="ztryMenuEnable"> ZONECENTRAL</em>
									</div>
								</li>
								<li class="line" id="zoneArea01">
									<div class="item">
										<div class="checkWrap">
											<div>
												<input type="checkbox" name="" id="zopenArea" disabled>
												<label data-i18n="agentPolicy.agentStatusViewPopup.showOpenArea">오픈 영역 표시</label>
											</div>
											<div>
												<input type="checkbox" name="" id="zpersonalAccess" disabled>
												<label data-i18n="agentPolicy.agentStatusViewPopup.showPersonalAccess">개인 접근 표시</label>
											</div>
											<div>
												<input type="checkbox" name="" id="zoptionDisplay" disabled>
												<label data-i18n="agentPolicy.agentStatusViewPopup.showOptions">옵션 표시</label>
											</div>
											<div>
												<input type="checkbox" name="" id="zsafetyZone" disabled>
												<label data-i18n="agentPolicy.agentStatusViewPopup.createSafetyZone">SafetyZone 만들기</label>
											</div>
											<div>
												<input type="checkbox" name="" id="zareaClose" disabled>
												<label data-i18n="agentPolicy.agentStatusViewPopup.closeAllAreas">모든 영역 닫기</label>
											</div>
											<div>
												<input type="checkbox" name="" id="zenc" disabled>
												<label data-i18n="agentPolicy.agentStatusViewPopup.encryptionCheck">암호화 확인</label>
											</div>
										</div>
									</div>
								</li>
								<!-- <li class="line">
									<div class="item">
										<em class="em100Per"><input type="checkbox" name="" id="ori01">ORIZON</em>
									</div>
								</li>
								<li class="line" id="oriArea01">
									<div class="item">
										<div class="checkWrap">
											<div>
												<input type="checkbox" name="" id="agentStatusViewPopupOpenORIZONFileExplorer" disabled>
												<label data-i18n="agentPolicy.agentStatusViewPopup.openORIZONFileExplorer">ORIZON 파일 탐색기 열기</label>
											</div>
											<div>
												<input type="checkbox" name="" id="agentStatusViewPopupEncryptionCheckAgain" disabled>
												<label data-i18n="agentPolicy.agentStatusViewPopup.encryptionCheckAgain">암호화 확인</label>
											</div>
										</div>
									</div>
								</li> -->
							</ul>
							<ul class="setWrap">
								<li class="line" id="fileTransfer">
									<div class="item">
										<div class="checkWrap">
											<div>
												<input type="checkbox" name="" id="agentStatusViewPopupExportFileAuth">
												<label data-i18n="agentPolicy.agentStatusViewPopup.createMoveFile">이동파일 만들기</label>
											</div>
											<div class="hide">
												<input type="checkbox" name="" id="agentStatusViewPopupFileTransfer">
												<label data-i18n="agentPolicy.agentStatusViewPopup.fileTransfer">파일 전송</label>
											</div>
											<div>
												<input type="checkbox" name="" id="agentStatusViewPopupWebManagerConsoleMenuEnable">
												<label data-i18n="agentPolicy.agentStatusViewPopup.userConsole">사용자 콘솔</label>
											</div>
										</div>
									</div>
								</li>
							</ul>
							<ul class="setWrap mb00">
								<li class="line" id="productMenu">
									<div class="item">
										<div class="checkWrap">
											<div>
												<input type="checkbox" name="" id="agentStatusViewPopupProductMenuEnable">
												<label data-i18n="agentPolicy.agentStatusViewPopup.PRIMXCSAgentInfo">PRIMXCS Agent정보</label>
											</div>
										</div>
									</div>
								</li>
							</ul>
						</li>
						<li>
							<ul class="setWrap">
								<li class="line">
									<div class="item">
										<em class="em100Per"><input type="checkbox" name="" id="fileZoneTableCheckbox"><span data-i18n="agentPolicy.agentStatusViewPopup.fileTransferZone"> 파일전송 Zone</span></em>
									</div>
								</li>
							</ul>
							<div class="tableWrap" id="fileZoneTableWrap">
								<table class="dataTable onlyCheckDataTable" id="fileZoneTable">
									<thead>
										<tr>
											<th width="5%"></th>
											<th width="20%" data-i18n="agentPolicy.agentStatusViewPopup.protocol">프로토콜</th>
											<th width="75%" data-i18n="agentPolicy.agentStatusViewPopup.zoneName">Zone명</th>
										</tr>
									</thead>
									<tbody>
										<!-- <tr>
											<td><input type="checkbox" name="chk_only" ></td>
											<td>HTTP curl</td>
											<td>반출 신청</td>
										</tr>
										<tr>
											<td><input type="checkbox" name="chk_only" ></td>
											<td>FTP</td>
											<td>영업부 공유 NAS</td>
										</tr>
										<tr>
											<td><input type="checkbox" name="chk_only" ></td>
											<td>HTTP Show</td>
											<td>그룹 TEAMS</td>
										</tr> -->
									</tbody>
								</table>
							</div>
						</li>
					</ul>
				</div>
				<div class="bu_wrap">
					<button class="modifyBtn" id="modifyBtn" type="button" data-i18n="button.edit" onclick="updateAgentStatus()">수정</button>
					<button class="saveBtn hide" id="saveBtn" type="button" data-i18n="button.save" onclick="saveAgentStatus()">저장</button>
					<button class="lineBtn" type="button" data-i18n="button.close" onclick="$('.agentStatusViewPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 삭제 -->
<section class="popup agnDelPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="agentPolicy.agnDelPopup.popTitle">삭제</strong>
		<form action="">
			<div class="pop_cont">
				<span data-i18n="agentPolicy.agnDelPopup.policyDeleteMessage">정책을 삭제하면 정책을 사용하고 있는 시스템은 기본 정책으로 변경됩니다.<br></span>
				<div class="bu_wrap">
					<button type="button" data-i18n="button.confirm" onclick="approveDeleteAgentPolicy();" >확인</button>
					<button type="button" class="lineBtn" data-i18n="button.cancel" onclick="$('.agnDelPopup').addClass('hide');" >취소</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 기본 정책 적용 -->
<section class="popup defaultChangePopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="agentPolicy.defaultChangePopup.popTitle">기본 정책 적용</strong>
		<form action="">
			<div class="pop_cont">
				<span data-i18n="agentPolicy.defaultChangePopup.defaultChangeTxt">선택한 정책을 기본 정책으로 변경하시겠습니까?<br></span>
				<div class="bu_wrap">
					<button type="button" data-i18n="button.confirm" onclick="applyDefaultAgentPolicy();">확인</button>
					<button type="button" class="lineBtn" data-i18n="button.cancel" onclick="$('.defaultChangePopup').addClass('hide');">취소</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 시스템 현황 -->
<section class="popup agnSysStatusPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head"><span id="polSysStatusPopupHeaderName"></span><span data-i18n="agentPolicy.agnSysStatusPopup.popTitle"> 시스템 현황</span></strong>
		<input type="hidden" id="polSysStatusPopupAgentPolicyCode" value="">
		<form action="">
			<div class="stickyWrap">
				<div class="searchInnerWrap_onlySc">
					<div class="searchBox">
						<input type="text" id="searchWord" class="txtBox searchInput" data-i18n-placeholder="agentPolicy.agnSysStatusPopup.searchSystemName" autocomplete="off" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchSystem();}">
						<button type="button" class="absolBtn" onclick="searchSystem()">검색</button>
					</div>
					<div class="dropMenu">
						<p class="selectMenu" data-i18n="menu.menu">메뉴</p>
						<ul>
							<li class="agnSysAddBtn"><button type="button" onclick="initAddSystem();" data-i18n="menu.addSystem">시스템 추가</button></li>
							<li class="agnSysDelBtn borbot"><button type="button" onclick="initDeleteSystem();" data-i18n="menu.removeSystem">시스템 제거</button></li>
							<li class="csv"><button type="button" id="agnSysStatusSysCsvExport" data-i18n="menu.csv">내보내기(CSV)</button></li>
						</ul>
					</div>
				</div>
			</div>
			<div class="pop_cont">
				<div class="agnSysStatusArea">
					<div class="tableWrap">
						<table class="dataTable polSysStatusTable" id="polSysStatusTable">
							<thead>
								<tr>
									<th width="3%"></th>
									<th width="5%" data-i18n="agentPolicy.agnSysStatusPopup.grid.number">번호</th>
									<th data-i18n="agentPolicy.agnSysStatusPopup.grid.systemID">시스템ID</th>
									<th width="12%" data-i18n="agentPolicy.agnSysStatusPopup.grid.systemName">시스템명</th>
									<th width="5%">ZC</th>
									<!-- <th width="3%">OZ</th> -->
									<th width="5%" data-i18n="agentPolicy.agnSysStatusPopup.grid.kind">종류</th>
									<th width="10%" data-i18n="agentPolicy.agnSysStatusPopup.grid.userName">사용자명</th>
									<th width="15%" data-i18n="agentPolicy.agnSysStatusPopup.grid.departmentName">부서명</th>
									<!-- <th width="9%" data-i18n="agentPolicy.agnSysStatusPopup.grid.windowsVersion">Window 버전</th>
									<th width="7%" data-i18n="agentPolicy.agnSysStatusPopup.grid.agentVersion">Agent 버전</th> -->
									<!-- <th width="7%" data-i18n="agentPolicy.agnSysStatusPopup.grid.policyName">정책명</th>
									<th width="7%" data-i18n="agentPolicy.agnSysStatusPopup.grid.registrationDate">등록일</th>
									<th width="9%" data-i18n="agentPolicy.agnSysStatusPopup.grid.lastLoginDateTime">최종접속일시</th> -->
								</tr>
							</thead>
							<tbody>
								<!-- <tr>
									<td><input type="checkbox" name="chk_pop" ></td>
									<td>1</td>
									<td>CAFDCC14-2DFC-4E8A-A358-51FBAE009F2C</td>
									<td>DESKTOP-6Q8S9V7</td>
									<td><span class="checkIco" title="">설치</span></td>
									<td><span class="checkIco" title="">설치</span></td>
									<td>사용자</td>
									<td>김소정</td>
									<td>기술연구소</td>
									<td>Windows 10 Pro</td>
									<td>1.0.0.7</td>
									<td>기본 정책</td>
									<td>2024-08-21</td>
									<td>2024-08-21 19:32:01</td>
								</tr> -->
							</tbody>
						</table>
					</div>
				</div>
				<div class="bu_wrap">
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.agnSysStatusPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 시스템 현황 > 시스템 추가 -->
<section class="popup agnSysStatusAddPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head"><span><!-- AGN-2024102100002 >  --></span><span data-i18n="agentPolicy.agnSysStatusAddPopup.popTitle"> 시스템 추가</span></strong>
		<form action="">
			<div class="stickyWrap">
				<div class="searchInnerWrap_onlySc">
					<div class="searchBox">
						<input type="text" id="searchWordAddSystem" class="txtBox searchInput" data-i18n-placeholder="agentPolicy.agnSysStatusAddPopup.searchSystemName" autocomplete="off" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchAddSystem();}">
						<button type="button" class="absolBtn" onclick="searchAddSystem()">검색</button>
						<span class="questionMark">
							<span class="questionTxt" style="width: max-content;" data-i18n="agentPolicy.agnSysStatusAddPopup.addSysTxt">정책을 적용할 시스템을 선택하세요.</span>
						</span>
					</div>
				</div>
			</div>
			<div class="pop_cont">
				<div class="agnSysStatusAddArea">
					<div class="tableWrap">
						<table class="dataTable agnSysStatusAddTable" id="agnSysStatusAddTable">
							<thead>
							<tr>
								<th width="3%"></th>
								<th width="5%" data-i18n="agentPolicy.agnSysStatusAddPopup.grid.number">번호</th>
								<th data-i18n="agentPolicy.agnSysStatusAddPopup.grid.systemID">시스템ID</th>
								<th width="12%" data-i18n="agentPolicy.agnSysStatusAddPopup.grid.systemName">시스템명</th>
								<th width="5%">ZC</th>
								<!-- <th width="3%">OZ</th> -->
								<th width="5%" data-i18n="agentPolicy.agnSysStatusAddPopup.grid.kind">종류</th>
								<th width="10%" data-i18n="agentPolicy.agnSysStatusAddPopup.grid.userName">사용자명</th>
								<th width="15%" data-i18n="agentPolicy.agnSysStatusAddPopup.grid.departmentName">부서명</th>
								<!-- <th width="9%" data-i18n="agentPolicy.agnSysStatusAddPopup.grid.windowsVersion">Window 버전</th>
								<th width="7%" data-i18n="agentPolicy.agnSysStatusAddPopup.grid.agentVersion">Agent 버전</th> -->
								<!-- <th width="7%" data-i18n="agentPolicy.agnSysStatusAddPopup.grid.policyName">정책명</th>
								<th width="7%" data-i18n="agentPolicy.agnSysStatusAddPopup.grid.registrationDate">등록일</th>
								<th width="9%" data-i18n="agentPolicy.agnSysStatusAddPopup.grid.lastLoginDateTime">최종접속일시</th> -->
							</tr>
						</thead>
						<tbody>
							<!-- <tr class="disTr">
								<td></td>
								<td>1</td>
								<td>CAFDCC14-2DFC-4E8A-A358-51FBAE009F2C</td>
								<td>DESKTOP-6Q8S9V7</td>
								<td><span class="checkIco" title="">설치</span></td>
								<td><span class="checkIco" title="">설치</span></td>
								<td>사용자</td>
								<td>김소정</td>
								<td>기술연구소</td>
								<td>Windows 10 Pro</td>
								<td>1.0.0.7</td>
								<td>기본 정책</td>
								<td>2024-08-21</td>
								<td>2024-08-21 19:32:01</td>
							</tr>
							<tr>
								<td></td>
								<td>2</td>
								<td>CAFDCC14-2DFC-4E8A-A358-51FBAE009F2C</td>
								<td>DESKTOP-6Q8S9V7</td>
								<td><span class="checkIco" title="">설치</span></td>
								<td><span class="checkIco" title="">설치</span></td>
								<td>사용자</td>
								<td>김소정</td>
								<td>기술연구소</td>
								<td>Windows 10 Pro</td>
								<td>1.0.0.7</td>
								<td>기본 정책</td>
								<td>2024-08-21</td>
								<td>2024-08-21 19:32:01</td>
							</tr> -->
						</tbody>
						</table>
					</div>
				</div>
				<div class="bu_wrap">
					<button type="button" data-i18n="button.add" onclick="addSystem();">추가</button>
					<button type="button" class="lineBtn" data-i18n="button.cancel" onclick="$('.agnSysStatusAddPopup').addClass('hide');">취소</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 시스템 현황 > 시스템 삭제 -->
<section class="popup agnSysDelPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="agentPolicy.agnSysDelPopup.popTitle">시스템 제거</strong>
		<form action="">
			<div class="pop_cont">
				<span data-i18n="agentPolicy.agnSysDelPopup.delSysTxt">선택된 시스템 정책을 제거 하시겠습니까?<br></span>
				<div class="bu_wrap">
					<button type="button" data-i18n="button.confirm" onclick="deleteSystem();" >확인</button>
					<button type="button" class="lineBtn" data-i18n="button.cancel" onclick="$('.agnSysDelPopup').addClass('hide');" >취소</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 검색 팝업 -->
<section class="popup searchAreaPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="search.searchTit">검색</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="search_wrap">
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.period">기간</label>
							<div class="selectBox">
								<select class="drop" name="dateSelect" id="dateSelect" data-label="기간">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="today" data-i18n="search.option.today">오늘</option>
									<option value="this-week" data-i18n="search.option.week">이번주</option>
									<option value="this-month" data-i18n="search.option.month">이번 달</option>
									<option value="custom" data-i18n="search.option.specifyPeriod">기간지정</option>
								</select>
							</div>
							<input type="text" class="datepicker datePickerInner txtBox start-date" name="startDt" id="startDt" data-i18n-placeholder="datepicker.startDate" readonly />
							<input type="text" class="datepicker datePickerInner txtBox end-date" name="endDt" id="endDt" data-i18n-placeholder="datepicker.endDate" readonly />
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.polType">유형</label>
							<div class="selectBox">
								<select class="drop" name="searchPolicyType" id="searchPolicyType">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="Y" data-i18n="search.option.default">기본</option>
									<option value="N" data-i18n="search.option.individual">개별</option>
								</select>
							</div>
						</div>
					</li>
					<li class="line">
						<div class="item">
							<label data-i18n="search.searchSubjects.condition">조건</label>
							<div class="selectBox">
								<select class="drop" name="searchCondition" id="searchCondition">
									<option value="all" data-i18n="search.option.all">전체</option>
									<option value="policyName" data-i18n="search.option.policyName">정책명</option>
									<option value="insertBy" data-i18n="search.option.registrant">등록자</option>
								</select>
							</div>
							<input type="text" class="txtBox w330" name="searchKeyword" id="searchKeyword" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchAgentStatus();}" />
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" onclick="searchAgentStatus();" data-i18n="button.search">검색</button>
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.searchAreaPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/pol/agentStatus.js' />"></script>