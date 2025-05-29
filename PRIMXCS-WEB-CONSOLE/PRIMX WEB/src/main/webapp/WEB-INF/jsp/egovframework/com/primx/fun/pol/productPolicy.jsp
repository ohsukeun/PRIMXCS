<%@ page pageEncoding="utf-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<title data-i18n="webTitle.productPolicy">PRIMX | 제품 정책</title>
<form class="productPolicy" action="">
	<div class="stickyWrap">
		<div class="titleWrap">
			<h2 class="title" data-i18n="navigation.productPolicy"></h2>
			<h2 class="depthTitle">
				<span class="fir" data-i18n="navigation.policy"></span>
				<span class="fir"> > </span>
				<span class="sec" data-i18n="navigation.productPolicy"></span>
			</h2>
		</div>
		<div class="searchInnerWrap">
			<div class="searchBox">
				<input type="text" class="txtBox searchInput" readonly data-i18n-placeholder="search.searchTit">
				<!-- <p class="filterItem">
					<span class="filterTitle">상태</span>
					<span> : </span>
					<span class="filterSelect">완료</span>
					<span class="delBtn" onclick="removeFilter(this);"></span>
				</p> -->
			</div>
			<div class="dropMenu">
				<p class="selectMenu" data-i18n="menu.menu">메뉴</p>
				<ul>
					<li class="polAddBtn"><button type="button" data-i18n="menu.register" onclick="openProductBasePolicyPopup();">등록</button></li>
					<li class="polCopyBtn"><button type="button" data-i18n="menu.copyPolicy" onclick="copyPolicy();">정책 복사</button></li>
					<li class="polDelBtn borbot"><button type="button" onclick="initDeletePolicy();" data-i18n="menu.delete">삭제</button></li>
					<li class="defaultChangeBtn borbot"><button type="button" onclick="initDefaultPolicyChange();" data-i18n="menu.applyDefaultPolicy">기본 정책 적용</button></li>
					<li class="sysStatusBtn borbot"><button type="button" onclick="openSystemStatusPopup();" data-i18n="menu.systemStatus">시스템 현황</button></li>
					<li class="csv"><button type="button" id="productPolicyCsvExport" data-i18n="menu.csv">내보내기(CSV)</button></li>
				</ul>
			</div>
		</div>
	</div>
	<div class="tableWrap">
		<table class="dataTable" id="productPolicyTable">
			<thead>
				<tr>
					<th width="3%" data-i18n="productPolicy.grid.select">선택</th>
					<th width="3%" data-i18n="productPolicy.grid.number">번호</th>
					<th width="5%" data-i18n="productPolicy.grid.polType">유형</th>
					<th width="10%" data-i18n="productPolicy.grid.product">제품</th>
					<th width="" data-i18n="productPolicy.grid.policyID">정책 ID</th>
					<th width="10%" data-i18n="productPolicy.grid.policyName">정책명</th>
					<th width="5%" data-i18n="productPolicy.grid.apply">적용</th>
					<th width="10%" data-i18n="productPolicy.grid.registrant">등록자</th>
					<th width="5%" data-i18n="productPolicy.grid.registrationDate">등록일</th>
					<th width="7%" data-i18n="productPolicy.grid.modificationDate">수정일</th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td></td>
					<td>1</td>
					<td class="result" title=""><span class="defaultPol">기본</span></td>
					<td>ZONECENTRAL</td>
					<td>POL-2024102100002</td>
					<td>기본 정책</td>
					<td>2</td>
					<td>admin 관리자</td>
					<td>2024-08-21</td>
					<td>2024-09-21</td>
				</tr>
				<tr>
					<td></td>
					<td>2</td>
					<td class="result" title=""><span class="eachPol">개별</span></td>
					<td>ZONECENTRAL</td>
					<td>POL-2024102100001</td>
					<td>개별 정책</td>
					<td>4</td>
					<td>admin 관리자</td>
					<td>2024-08-21</td>
					<td>2024-09-21</td>
				</tr>
			</tbody>
		</table>
	</div>
</form>

<!-- 제품 정책 팝업 -->
<section class="popup productBasePolicyPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="productPolicy.productBasePolicyPopup.popTitle">제품 정책</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="setWrap topSetWrap">
					<li class="line w2">
						<div class="item">
							<em data-i18n="productPolicy.productBasePolicyPopup.product">제품 *</em>
							<div class="">
								<div class="selectBox">
									<select class="drop" name="" id="productSelect">
										<option value="ZONECENTRAL">ZONECENTRAL</option>
									</select>
								</div>
							</div>
						</div>
						<div class="item">
							<em data-i18n="productPolicy.productBasePolicyPopup.policy_selection">정책 선택</em>
							<div class="">
								<div class="selectBox wAuto polSelectBox">
									<select class="drop" name="" id="policySelect">
										<option value="fix" data-i18n="productPolicy.productBasePolicyPopup.fixed_policy">고정 정책</option>
										<option value="132" data-i18n="productPolicy.productBasePolicyPopup.safety_zone_area_132">SafetyZone영역(132)</option>
										<option value="354" data-i18n="productPolicy.productBasePolicyPopup.safety_zone_access_process_354">SafetyZone접근 프로세스(354)</option>
										<option value="153" data-i18n="productPolicy.productBasePolicyPopup.user_interface_153">사용자 인터페이스(153)</option>
									</select>
								</div>
							</div>
						</div>
					</li>
					<li class="line w2">
						<div class="item">
							<em data-i18n="productPolicy.productBasePolicyPopup.policy_name">정책명 *</em>
							<div class="">
								<input type="text" class="txtBox" name="" placeholder="" id="policyNameInput">
								<input type="hidden" id="policyCodeInput">
							</div>
						</div>
						<div class="item">
							<em data-i18n="productPolicy.productBasePolicyPopup.policy_type">정책 유형</em>
							<div class="">
								<input type="text" class="txtBox" name="" placeholder="개별" id="policyTypeInput" readonly>
							</div>
						</div>
					</li>
				</ul>
				<div class="divisionWrap">
					<div class="treeArea hide">
						<div id="tree"></div>
					</div>
					<div class="polArea hide">
						<div id="policy-details">
							<div id="policy-settings">
							</div>
						</div>
					</div>
					<div class="polDataArea">
						<div class="fixedPol polSection">
							<div class="titleWrap">
								<h2 class="title" data-i18n="productPolicy.productBasePolicyPopup.fixed_policy">고정 정책</h2>
							</div>
							<div class="contDiv">
								<h3 class="contTitle" data-i18n="productDefaultPolicySettings.path">경로</h3>
								<ul class="setWrap">
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.personalZafPath120">개인Zaf 경로 (120)</em>
											<div>
												<input type="text" class="txtBox" value="" id="default120" disabled>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.zafPath121">Zaf 경로 (121)</em>
											<div>
												<input type="text" class="txtBox" value="" id="default121" disabled>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.zafCachePath122">Zaf Cache 경로 (122)</em>
											<div>
												<input type="text" class="txtBox" value="" id="default122" disabled>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.backupPath127">백업 경로 (127)</em>
											<div>
												<input type="text" class="txtBox" value="" id="default127" disabled>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.logFilePath309">로그파일 경로 (309)</em>
											<div>
												<input type="text" class="txtBox" value="" id="default309" disabled>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.zonemapPath343">Zonemap 경로 (343)</em>
											<div>
												<input type="text" class="txtBox" value="" id="default343" disabled>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.keymapPath345">Keymap 경로 (345)</em>
											<div>
												<input type="text" class="txtBox" value="" id="default345" disabled>
											</div>
										</div>
									</li>
								</ul>
							</div>
							<div class="contDiv">
								<h3 class="contTitle" data-i18n="productDefaultPolicySettings.fixedAndReserved">고정 및 예약</h3>
								<h4 class="contSubTitle" data-i18n="productDefaultPolicySettings.requiredZaf131">필수 Zaf (131)</h4>
								<div class="tableWrap">
									<table id="default131">
										<thead>
											<tr>
												<th width="30%" data-i18n="productDefaultPolicySettings.grid.name">이름</th>
												<th data-i18n="productDefaultPolicySettings.grid.value">값</th>
											</tr>
										</thead>
										<tbody>
											<tr>
												<td><input type="text" name="" class="txtBox w100Per" value="Mandatory.zaf" disabled></td>
												<td><input type="text" name="" class="txtBox w100Per_10" value="" disabled></td>
											</tr>
											<tr>
												<td><input type="text" name="" class="txtBox w100Per" value="Helpdesk.zaf" disabled></td>
												<td><input type="text" name="" class="txtBox w100Per_10" value="" disabled></td>
											</tr>
										</tbody>
									</table>
								</div>
								<h4 class="contSubTitle" data-i18n="productDefaultPolicySettings.reserved499">예약 (499)</h4>
								<div class="tableWrap">
									<table id="default499">
										<thead>
											<tr>
												<th width="30%" data-i18n="productDefaultPolicySettings.grid.name">이름</th>
												<th width="70%" data-i18n="productDefaultPolicySettings.grid.value">값</th>
											</tr>
										</thead>
										<tbody>
											<tr>
												<td title=""><input type="text" name="" class="txtBox w100Per" value="" disabled></td>
												<td title=""><input type="text" name="" class="txtBox w100Per_10" value="" disabled></td>
											</tr>
										</tbody>
									</table>
								</div>
								<h4 class="contSubTitle" data-i18n="productDefaultPolicySettings.disablePersonalPolicy2">개인정책 비활성화 (2)</h4>
								<div class="selectBox">
									<select class="drop" name="" id="default2" disabled>
										<option data-i18n="boolean.y" value="Y">예</option>
										<option data-i18n="boolean.n" value="N">아니오</option>
									</select>
								</div>
							</div>
							<div class="contDiv">
								<h3 class="contTitle" data-i18n="productDefaultPolicySettings.agentIntegration">Agent 연동</h3>
								<ul class="setWrap">
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.productTrayHide197">제품 트레이 숨김 (197)</em>
											<div>
												<div class="selectBox">
													<select class="drop" name="" id="default197" disabled>
														<option data-i18n="boolean.y" value="Y">예</option>
														<option data-i18n="boolean.n" value="N">아니오</option>
													</select>
												</div>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.disableAPI392">API 사용금지 (392)</em>
											<div>
												<div class="selectBox">
													<select class="drop" name="" id="default392" disabled>
														<option data-i18n="boolean.y" value="Y">예</option>
														<option data-i18n="boolean.n" value="N">아니오</option>
													</select>
												</div>
											</div>
										</div>
									</li>
								</ul>
							</div>
							<div class="contDiv">
								<h3 class="contTitle" data-i18n="productDefaultPolicySettings.accessType">접근 유형</h3>
								<ul class="setWrap">
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.disablePKCS12KeyFileAccess103">암호화된 영역/컨테이너 PKCS#12 키 파일 접근 비활성화(103)</em>
											<div>
												<div class="selectBox">
													<select class="drop" name="" id="default103" disabled>
														<option data-i18n="boolean.y" value="Y">예</option>
														<option data-i18n="boolean.n" value="N">아니오</option>
													</select>
												</div>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.disableCSPProviderAccess105">암호화된 영역/컨테이너 CSP 공급자 접근 비활성화(105)</em>
											<div>
												<div class="selectBox">
													<select class="drop" name="" id="default105" disabled>
														<option data-i18n="boolean.y" value="Y">예</option>
														<option data-i18n="boolean.n" value="N">아니오</option>
													</select>
												</div>
											</div>
										</div>
									</li>
								</ul>
							</div>
							<div class="contDiv">
								<h3 class="contTitle" data-i18n="productDefaultPolicySettings.personalAccessList">개인접근 목록</h3>
								<ul class="setWrap">
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.personalAccessList123">개인접근 목록(123)</em>
											<div>
												<div class="selectBox">
													<select class="drop" name="" id="default123" disabled>
														<option data-i18n="boolean.y" value="Y">예</option>
														<option data-i18n="boolean.n" value="N">아니오</option>
													</select>
												</div>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.disableCreatePersonalAccessList124">사용자 개인 접근 목록 생성 불가(124)</em>
											<div>
												<div class="selectBox">
													<select class="drop" name="" id="default124" disabled>
														<option data-i18n="boolean.y" value="Y">예</option>
														<option data-i18n="boolean.n" value="N">아니오</option>
													</select>
												</div>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.syncPersonalAccessListWithDefaultLocation125">개인 접근 목록을 기본 위치와 동기화(125)</em>
											<div>
												<div class="selectBox">
													<select class="drop" name="" id="default125" disabled>
														<option data-i18n="boolean.y" value="Y">예</option>
														<option data-i18n="boolean.n" value="N">아니오</option>
													</select>
												</div>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.personalAccessListNameFormat126">개인 접근 목록 이름 형식 (126)</em>
											<div>
												<input type="text" class="txtBox" id="default126" value="" disabled>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.createPersonalAccessListOnSessionStart5120">세션 시작시 개인 접근 목록 생성(5120)</em>
											<div>
												<div class="selectBox">
													<select class="drop" name="" id="default5120" disabled>
														<option data-i18n="boolean.y" value="Y">예</option>
														<option data-i18n="boolean.n" value="N">아니오</option>
													</select>
												</div>
											</div>
										</div>
									</li>
								</ul>
							</div>
							<div class="contDiv">
								<h3 class="contTitle" data-i18n="productDefaultPolicySettings.logs">로그</h3>
								<ul class="setWrap">
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.logOpenCloseAreaEvents302">영역 열기 및 닫기 이벤트에 대한 로깅 켜기(302)</em>
											<div>
												<div class="selectBox">
													<select class="drop" name="" id="default302" disabled>
														<option data-i18n="boolean.y" value="Y">예</option>
														<option data-i18n="boolean.n" value="N">아니오</option>
													</select>
												</div>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.logAccessToEncryptedFiles306">암호화된 파일에 대한 접근 로그(306)</em>
											<div>
												<div class="selectBox">
													<select class="drop" name="" id="default306" disabled>
														<option data-i18n="boolean.y" value="Y">예</option>
														<option data-i18n="boolean.n" value="N">아니오</option>
													</select>
												</div>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.logDeniedAccessToEncryptedFiles307">암호화된 파일에 대한 거부된 접근 기록(307)</em>
											<div>
												<div class="selectBox">
													<select class="drop" name="" id="default307" disabled>
														<option data-i18n="boolean.y" value="Y">예</option>
														<option data-i18n="boolean.n" value="N">아니오</option>
													</select>
												</div>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="productDefaultPolicySettings.logDeniedCreationOfPlainTextFiles308">일반 텍스트 파일 생성이 거부된 로그(308)</em>
											<div>
												<div class="selectBox">
													<select class="drop" name="" id="default308" disabled>
														<option data-i18n="boolean.y" value="Y">예</option>
														<option data-i18n="boolean.n" value="N">아니오</option>
													</select>
												</div>
											</div>
										</div>
									</li>
								</ul>
							</div>
						</div>
						<div class="pol132 polSection hide">
							<div class="titleWrap">
								<h2 class="title checkTitle"><input type="checkbox" name="" id="default132Checkbox"><span data-i18n="productPolicy.productBasePolicyPopup.safety_zone_area_132"> SafetyZone 영역(132)</span></h2>
							</div>
							<div class="tableWrap mb10">
								<div class="bu_wrap tableBtnPosAbsolute">
									<button class="" type="button" onclick="addRow('#default132Checkbox', 'default132')">+</button>
									<button class="" type="button" onclick="deleteRow('#default132Checkbox', 'default132')">-</button>
								</div>
								<table id="default132">
									<thead>
										<tr>
											<th width="30%" data-i18n="productDefaultPolicySettings.grid.name">이름</th>
											<th data-i18n="productDefaultPolicySettings.grid.value">값</th>
										</tr>
									</thead>
									<tbody>
										<!--<tr>
											<td><input type="text" name="" class="txtBox w100Per" value="*" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="*={denyreadencrypted=1}" disabled></td>
										</tr>
										<tr>
											<td><input type="text" name="" class="txtBox w100Per" value="explorer.exe" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="*={denyreadencrypted=1}" disabled></td>
										</tr>
										<tr>
											<td><input type="text" name="" class="txtBox w100Per" value="winword.exe" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="*={denyreadencrypted=0;denywriteclear=1};%USERPROFILE%\AppData={denywriteclear =0}" disabled></td>
										</tr>-->
									</tbody>
								</table>
							</div>
							<ul class="setWrap">
								<li class="line">
									<div class="item">
										<em data-i18n="productPolicy.productBasePolicyPopup.133">위탁품이 있는 사용자(133)</em>
										<div>
											<input type="text" class="txtBox" value="" id="default133" disabled>
										</div>
									</div>
								</li>
								<li class="line">
									<div class="item">
										<em data-i18n="productPolicy.productBasePolicyPopup.134">위탁품이 없는 사용자(134)</em>
										<div>
											<input type="text" class="txtBox" value="" id="default134" disabled>
										</div>
									</div>
								</li>
								<li class="line">
									<div class="item">
										<em data-i18n="productPolicy.productBasePolicyPopup.135">자동 위탁 비활성화(135)</em>
										<div>
											<div class="selectBox wAuto">
												<select class="drop" name="" id="default135" disabled>
													<option value="0">0 - At each login</option>
													<option value="1">1- Never</option>
													<option value="2">2- Upon non-compliance </option>
												</select>
											</div>
										</div>
									</div>
								</li>
							</ul>
						</div>
						<div class="pol354 polSection hide">
							<div class="titleWrap">
								<h2 class="title checkTitle"><input type="checkbox" name="" id="default354Checkbox"><span data-i18n="productPolicy.productBasePolicyPopup.safety_zone_access_process_354"> SafetyZone 접근 프로세스(354)</span></h2>
							</div>
							<div class="tableWrap">
								<div class="bu_wrap tableBtnPosAbsolute">
									<button class="" type="button" onclick="addRow('#default354Checkbox', 'default354')">+</button>
									<button class="" type="button" onclick="deleteRow('#default354Checkbox', 'default354')">-</button>
								</div>
								<table id="default354">
									<thead>
										<tr>
											<th width="30%" data-i18n="productDefaultPolicySettings.grid.name">이름</th>
											<th data-i18n="productDefaultPolicySettings.grid.value">값</th>
										</tr>
									</thead>
									<tbody>
										<!--<tr>
											<td><input type="text" name="" class="txtBox w100Per" value="*" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="*={denyreadencrypted=1}" disabled></td>
										</tr>
										<tr>
											<td><input type="text" name="" class="txtBox w100Per" value="explorer.exe" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="*={denyreadencrypted=1}" disabled></td>
										</tr>
										<tr>
											<td><input type="text" name="" class="txtBox w100Per" value="winword.exe" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="*={denyreadencrypted=0;denywriteclear=1};%USERPROFILE%\AppData={denywriteclear =0}" disabled></td>
										</tr>-->
									</tbody>
								</table>
							</div>
						</div>
						<div class="pol153 polSection hide">
							<div class="titleWrap">
								<h2 class="title checkTitle"><input type="checkbox" name="" id="default153Checkbox"><span data-i18n="productPolicy.productBasePolicyPopup.user_interface_153"> 사용자 인터페이스(153)</span></h2>
							</div>
							<div class="tableWrap">
								<table id="default153">
									<thead>
										<tr>
											<th width="5%" data-i18n="productPolicy.productBasePolicyPopup.grid.select">선택</th>
											<th width="30%" data-i18n="productPolicy.productBasePolicyPopup.grid.code">코드</th>
											<th data-i18n="productPolicy.productBasePolicyPopup.grid.pol">정책</th>
										</tr>
									</thead>
									<tbody>
										<tr>
											<td><input type="checkbox" name="" value="" id="default158" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="158" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Zonecentral 모니터의 '개인접근' 탭" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default159" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="159" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) 'Personal access key' tab in the monitor" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default160" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="160" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) 폴더 컨텍스트 메뉴 (SafetyZone 생성/해제)" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default161" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="161" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Operations not granted by the 'Admin rights' policies (P231+)" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default162" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="162" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) SafetyZone 생성 중요 메뉴" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default163" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="163" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) SafetyZone 해제 중요 메뉴" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default164" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="164" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Verify operation" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default165" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="165" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Transcrypt operation" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default166" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="166" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) 엑세스 목록" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default167" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="167" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) 엑세스 목록 작업(추가, 수정, 속성)" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default168" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="168" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) 패스워드 엑세스 추가" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default169" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="169" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) 인증서 엑세스 파일로 추가" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default170" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="170" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) 인증서 엑세스 윈도우 스토어에서 추가" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default171" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="171" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) 인증서 엑세스 LDAP 디렉토리에서 추가" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default172" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="172" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) SafetyZone 속성의 암호화탭 고급/상세 설정" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default173" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="173" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Zone general characteristics" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default174" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="174" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Zone properties change" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default175" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="175" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Clear zone creation" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default176" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="176" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Group operation" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default177" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="177" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Ungroup operation" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default178" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="178" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) SafetyZone 속성의 암호화탭 고급의 예외 확장자 탭" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default179" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="179" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) SafetyZone 속성의 암호화탭 고급의 예외 확장자 탭의 확장자 수정" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default180" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="180" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) 하위 영역 검색" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default181" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="181" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Show SOS access" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default182" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="182" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Display details of a check" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default183" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="183" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Workstation operations" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default184" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="184" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) 엑세스 목록 추가" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default185" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="185" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Repair operation" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default218" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="218" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Add Password access for encrypted containers" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default219" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="219" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Add certificate access (from a file) for encrypted containers" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default220" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="220" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Add certificate access (from a Windows certstore) for encrypted containers" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default221" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="221" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Add certificate access (from a LDAP directory) for encrypted containers" disabled></td>
										</tr>
										<tr>
											<td><input type="checkbox" name="" value="" id="default222" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per" value="222" disabled></td>
											<td><input type="text" name="" class="txtBox w100Per_10" value="(숨김) Add access lists for encrypted containers" disabled></td>
										</tr>
									</tbody>
								</table>
							</div>
						</div>
					</div>
				</div>
				
				<div class="bu_wrap">
					<button type="button" id="saveBtn" class="" data-i18n="button.save" onclick="saveProductPolicy()">저장</button>
					<button type="button" id="updateBtn" class="hide" data-i18n="button.edit" onclick="updateProductPolicy()">수정</button>
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="popupPolicyClear()">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 제품 정책 팝업(tree) -->
<!-- <section class="popup productPolicyPopup ">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head">등록</strong>
		<form action="">
			<div class="pop_cont">
				<ul class="setWrap">
					<li class="line">
						<div class="item">
							<em>제품 *</em>
							<div class="">
								<div class="selectBox">
									<select class="drop" name="">
										<option value="ZONECENTRAL">ZONECENTRAL</option>
									</select>
								</div>
							</div>
						</div>
					</li>
					<li class="line w2">
						<div class="item">
							<em>정책명 *</em>
							<div class="">
								<input type="text" class="txtBox" name="" placeholder="">
							</div>
						</div>
						<div class="item">
							<em>정책 유형</em>
							<div class="">
								<input type="text" class="txtBox" name="" placeholder="개별" readonly>
							</div>
						</div>
					</li>
				</ul>
				<div class="flexWrap sbStart alignCenter mb20">
					<h3 class="contTitle mb00 mr10">정책 XML *</h3>
					<div class="bu_wrap">
						<button type="button" class="lineBtn">< ></button>
					</div>
				</div>
				<div class="divisionWrap">
					<div class="treeArea">
						<div id="tree"></div>
					</div>
					<div class="polArea">
						<div id="policy-details">
							<div id="policy-settings">
							</div>
						</div>
					</div>
				</div>
				
				<div class="bu_wrap">
					<button type="button" class="lineBtn" data-i18n="button.close">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section> -->

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
							<label data-i18n="search.searchSubjects.product">제품</label>
							<div class="selectBox">
								<select class="drop" name="searchProduct" id="searchProduct">
									<option value="" data-i18n="search.option.all">전체</option>
									<option value="ZONECENTRAL">ZONECENTRAL</option>
									<!-- <option value="ORIZON">ORIZON</option> -->
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
									<option value="createdBy" data-i18n="search.option.registrant">등록자</option>
								</select>
							</div>
							<input type="text" class="txtBox w330" name="searchKeyword" id="searchKeyword" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchProductPolicy();}" />
						</div>
					</li>
				</ul>
				<div class="bu_wrap">
					<button type="button" onclick="searchProductPolicy()" data-i18n="button.search">검색</button>
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.searchAreaPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 삭제 -->
<section class="popup polDelPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="productPolicy.polDelPopup.popTitle">삭제</strong>
		<form action="">
			<div class="pop_cont">
				<span data-i18n="productPolicy.polDelPopup.policyDeleteMessage">정책을 삭제하면 정책을 사용하고 있는 시스템은 기본 정책으로 변경됩니다.<br></span>
				
				<div class="bu_wrap">
					<button type="button" data-i18n="button.confirm" onclick="deletePolicy();" >확인</button>
					<button type="button" class="lineBtn" data-i18n="button.cancel" onclick="$('.polDelPopup').addClass('hide');">취소</button>
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
		<strong class="pop_head" data-i18n="productPolicy.defaultChangePopup.popTitle">기본 정책 적용</strong>
		<form action="">
			<div class="pop_cont">
				<span data-i18n="productPolicy.defaultChangePopup.defaultChangeTxt">선택한 정책을 기본 정책으로 변경하시겠습니까?<br></span>
				<div class="bu_wrap">
					<button type="button" data-i18n="button.confirm" onclick="applyDefaultPolicy();">확인</button>
					<button type="button" class="lineBtn" data-i18n="button.cancel" onclick="$('.defaultChangePopup').addClass('hide');">취소</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 시스템 현황 -->
<section class="popup polSysStatusPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head"><span id="polSysStatusPopupHeaderName"></span><span data-i18n="productPolicy.polSysStatusPopup.popTitle"> 시스템 현황</span></strong>
		<input type="hidden" id="polSysStatusPopupPolicyCode" value="">
		<form action="">
			<div class="stickyWrap">
				<div class="searchInnerWrap_onlySc">
					<div class="searchBox">
						<input type="text" id="searchWord" class="txtBox searchInput" data-i18n-placeholder="productPolicy.polSysStatusPopup.searchSystemName" autocomplete="off" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchSystem();}">
						<button type="button" class="absolBtn" onclick="searchSystem()">검색</button>
					</div>
					<div class="dropMenu">
						<p class="selectMenu" data-i18n="menu.menu">메뉴</p>
						<ul>
							<li class="agnSysAddBtn"><button type="button" onclick="initAddSystem();" data-i18n="menu.addSystem">시스템 추가</button></li>
							<li class="agnSysDelBtn borbot"><button type="button" onclick="initDeleteSystem();" data-i18n="menu.removeSystem">시스템 제거</button></li>
							<li class="csv"><button type="button" id="polSysStatusCsvExport" data-i18n="menu.csv">내보내기(CSV)</button></li>
						</ul>
					</div>
				</div>
			</div>
			<div class="pop_cont">
				<div class="polSysStatusArea">
					<div class="tableWrap">
						<table class="dataTable" id="polSysStatusTable">
							<thead>
								<tr>
									<th width="3%"></th>
									<th width="5%" data-i18n="productPolicy.polSysStatusPopup.grid.number">번호</th>
									<th data-i18n="productPolicy.polSysStatusPopup.grid.systemID">시스템ID</th>
									<th width="12%" data-i18n="productPolicy.polSysStatusPopup.grid.systemName">시스템명</th>
									<th width="5%">ZC</th>
									<!-- <th width="3%">OZ</th> -->
									<th width="5%" data-i18n="productPolicy.polSysStatusPopup.grid.kind">종류</th>
									<th width="10%" data-i18n="productPolicy.polSysStatusPopup.grid.userName">사용자명</th>
									<th width="15%" data-i18n="productPolicy.polSysStatusPopup.grid.departmentName">부서명</th>
									<!-- <th width="9%" data-i18n="productPolicy.polSysStatusPopup.grid.windowsVersion">Window 버전</th>
									<th width="7%" data-i18n="productPolicy.polSysStatusPopup.grid.agentVersion">Agent 버전</th> -->
									<!-- <th width="7%" data-i18n="productPolicy.polSysStatusPopup.grid.policyName">정책명</th>
									<th width="7%" data-i18n="productPolicy.polSysStatusPopup.grid.registrationDate">등록일</th>
									<th width="9%" data-i18n="productPolicy.polSysStatusPopup.grid.lastLoginDateTime">최종접속일시</th> -->
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
					<button type="button" class="lineBtn" data-i18n="button.close" onclick="$('.polSysStatusPopup').addClass('hide');">닫기</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop" ></button>
	</div>
</section>

<!-- 시스템 현황 > 시스템 추가 -->
<section class="popup polSysStatusAddPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head"><span id="polSysStatusAddPopupHeaderName"></span><span data-i18n="productPolicy.polSysStatusAddPopup.popTitle"> 시스템 추가</span></strong>
		<input type="hidden" id="polSysStatusAddPopupPolicyCode" value="">
		<form action="">
			<div class="stickyWrap">
				<div class="searchInnerWrap_onlySc">
					<div class="searchBox">
						<input type="text" id="searchWordAddSystem" class="txtBox searchInput" data-i18n-placeholder="productPolicy.polSysStatusAddPopup.searchSystemName" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchAddSystem();}">
						<button type="button" class="absolBtn" onclick="searchAddSystem()">검색</button>
						<span class="questionMark">
							<span class="questionTxt" style="width: max-content;" data-i18n="productPolicy.polSysStatusAddPopup.addSysTxt">정책을 적용할 시스템을 선택하세요.</span>
						</span>
					</div>
				</div>
			</div>
			<div class="pop_cont">
				<div class="polSysStatusAddArea">
					<div class="tableWrap">
						<table class="dataTable" id="polSysStatusAddPopupTable">
							<thead>
								<tr>
									<th width="3%"></th>
									<th width="5%" data-i18n="productPolicy.polSysStatusAddPopup.grid.number">번호</th>
									<th data-i18n="productPolicy.polSysStatusAddPopup.grid.systemID">시스템ID</th>
									<th width="12%" data-i18n="productPolicy.polSysStatusAddPopup.grid.systemName">시스템명</th>
									<th width="5%">ZC</th>
									<!-- <th width="3%">OZ</th> -->
									<th width="5%" data-i18n="productPolicy.polSysStatusAddPopup.grid.kind">종류</th>
									<th width="10%" data-i18n="productPolicy.polSysStatusAddPopup.grid.userName">사용자명</th>
									<th width="15%" data-i18n="productPolicy.polSysStatusAddPopup.grid.departmentName">부서명</th>
									<!-- <th width="9%" data-i18n="productPolicy.polSysStatusAddPopup.grid.windowsVersion">Window 버전</th>
									<th width="7%" data-i18n="productPolicy.polSysStatusAddPopup.grid.agentVersion">Agent 버전</th> -->
									<!-- <th width="7%" data-i18n="productPolicy.polSysStatusAddPopup.grid.policyName">정책명</th>
									<th width="7%" data-i18n="productPolicy.polSysStatusAddPopup.grid.registrationDate">등록일</th>
									<th width="9%" data-i18n="productPolicy.polSysStatusAddPopup.grid.lastLoginDateTime">최종접속일시</th> -->
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
					<button type="button" class="lineBtn" data-i18n="button.cancel" onclick="$('.polSysStatusAddPopup').addClass('hide');">취소</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<!-- 시스템 현황 > 시스템 삭제 -->
<section class="popup polSysDelPopup hide">
	<div class="bgscreen"></div>
	<div class="pop_wrap">
		<strong class="pop_head" data-i18n="productPolicy.polSysDelPopup.popTitle">시스템 제거</strong>
		<form action="">
			<div class="pop_cont">
				<span data-i18n="productPolicy.polSysDelPopup.delSysTxt">선택된 시스템 정책을 제거 하시겠습니까?<br></span>
				<div class="bu_wrap">
					<button type="button" data-i18n="button.confirm" onclick="deleteSystem();">확인</button>
					<button type="button" class="lineBtn" data-i18n="button.cancel" onclick="$('.polSysDelPopup').addClass('hide');">취소</button>
				</div>
			</div>
		</form>
		<button class="bu_close_pop"></button>
	</div>
</section>

<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/dist/jstree.js' />"></script>
<script type="text/javascript" src="<c:url value='/js/egovframework/com/primx/fun/pol/productPolicy.js' />"></script>
