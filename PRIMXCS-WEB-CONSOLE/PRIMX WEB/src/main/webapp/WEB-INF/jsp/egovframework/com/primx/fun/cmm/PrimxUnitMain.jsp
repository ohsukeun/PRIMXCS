<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions" %>
<%@ taglib prefix="ui" uri="http://egovframework.gov/ctl/ui" %>
<%@ taglib prefix="spring" uri="http://www.springframework.org/tags"%>
<!DOCTYPE html>
<html lang="ko">
<head>
	<!-- head 인클루드 -->
	<%@ include file="./PrimxUnitHeader.jsp" %>

</head>

<body>
	<div class="division">
	    <!-- Top 영역 -->
		<%@ include file="./PrimxUnitTop.jsp" %>
		<!-- Left 영역 -->
		<%@ include file="./PrimxUnitLeft.jsp" %>
		<!-- Content 영역 -->
		<section class="content"></section>
	</div>

	<!-- 공통 사용 팝업 : 상단 알림 팝업 -->
	<section class="popup topAlarmPopup hide">
		<div class="bgscreen"></div>
		<div class="pop_wrap">
			<strong class="pop_head">
				<span data-i18n="topAlarmPopup.popTitle">알림</span>
			</strong>
			<form action="">
				<div class="pop_cont">
					<div class="bu_wrap topBtnWrap">
						<button type="button" class="allViewStatusBtn lineBtn" data-i18n="topAlarmPopup.markAllAsRead">모두 읽음</button>
					</div>
					<ul class="alarmWrap" id="alarmWrap">
						<!-- 여기에 알람 목록이 추가됩니다. -->
					</ul>
					<div class="bu_wrap moreBtnWrap">
						<button type="button" data-i18n="topAlarmPopup.more">더보기</button>
					</div>
				</div>
			</form>
			<button class="bu_close_pop"></button>
		</div>
	</section>
	
	<!-- 공통 사용 팝업 : 상단 설정 아이콘 팝업 -->
	<section class="popup topSetPopup hide">
		<div class="bgscreen"></div>
		<div class="pop_wrap">
			<strong class="pop_head" data-i18n="navigation.setting">설정</strong>
			<form action="">
				<div class="pop_cont">
					<div class="settabWrap">
						<ul class="tabTitle">
							<li class="on" data-i18n="navigation.topSetLicense">라이선스</li>
							<li data-i18n="navigation.topSetVerification">인증</li>
							<li data-i18n="navigation.operation">운영</li>
						</ul>
						<ul class="tabCont">
							<li class="on license">
								<div class="contDiv">
									<div class="flexWrap sbStart">
										<h3 class="contTitle" data-i18n="topSetLicense.companyInfo">회사 정보</h3>
										<div class="bu_wrap">
											<button class="licenseRegiBtn" type="button" onclick="openLicensePopup()" data-i18n="button.license">라이선스 등록</button>
										</div>
									</div>
									<ul class="setWrap">
										<li class="line w2">
											<div class="item">
												<em data-i18n="topSetLicense.companyName">회사명</em>
												<div>
													<input type="text" class="txtBox" id="companyName" disabled>
												</div>
											</div>
											<div class="item">
												<em data-i18n="topSetLicense.code">코드</em>
												<div>
													<input type="text" class="txtBox" id="companyCode" disabled>
												</div>
											</div>
										</li>
									</ul>
								</div>
								<div class="contDiv">
									<h3 class="contTitle" data-i18n="topSetLicense.licenseInfo">라이선스 정보</h3>
									<div class="tableWrap">
										<table class="dataTable normalDataTable" id="licenseDetailTable" style="width:100%">
											<thead>
												<tr>
													<th width="5%" data-i18n="topSetLicense.grid.number">번호</th>
													<th width="10%" data-i18n="topSetLicense.grid.status">상태</th>
													<th width="30%" data-i18n="topSetLicense.grid.productName">제품명</th>
													<th width="10%" data-i18n="topSetLicense.grid.quantity">수량</th>
													<th width="10%" data-i18n="topSetLicense.grid.startDate">시작일</th>
													<th width="10%" data-i18n="topSetLicense.grid.endDate">종료일</th>
												</tr>
											</thead>
											<tbody>
												<!-- <tr>
													<td>1</td>
													<td>사용</td>
													<td>ZONECENTRAL</td>
													<td>100</td>
													<td>2024-10-01</td>
													<td>2025-09-30</td>
												</tr>
												<tr>
													<td>2</td>
													<td>만료</td>
													<td>ORIZON</td>
													<td>100</td>
													<td>2024-10-01</td>
													<td>2025-09-30</td>
												</tr> -->
											</tbody>
										</table>
									</div>
								</div>			
								<!-- <div class="bu_wrap">
									<button data-i18n="button.save">저장</button>
								</div> -->
							</li>
							<li class="certified">
								<h3 class="contTitle" data-i18n="topSetVerification.userAccount">사용자 계정</h3>
								<ul class="setWrap">
									<li class="line">
										<div class="item">
											<em data-i18n="topSetVerification.authenticationMethod">인증 방법</em>
											<div>
												<div class="selectBox w230">
													<select class="drop" name="" >
														<option value="" data-i18n="topSetVerification.passwordAuthentication">비밀번호 인증</option>
													</select>
												</div>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="topSetVerification.complexity">복잡도</em>
											<div>
												<ul class="itemDivision">
													<li>
														<input type="checkbox" name="" id="minimumLengthCheckbox">
														<label for="minimumLengthCheckbox" data-i18n="topSetVerification.minimumLength">최소 길이</label>
														<input type="text" class="txtBox minInput" id="passMinLength">
													</li>
													<li>
														<input type="checkbox" name="" id="uppercaseCheckbox">
														<label for="uppercaseCheckbox" data-i18n="topSetVerification.uppercase">대문자</label>
														<input type="text" class="txtBox minInput" id="passCombUppercase">
													</li>
													<li>
														<input type="checkbox" name="" id="numbersCheckbox">
														<label for="numbersCheckbox" data-i18n="topSetVerification.numbers">숫자</label>
														<input type="text" class="txtBox minInput" id="passCombDigit">
													</li>
													<li>
														<input type="checkbox" name="" id="specialCharactersCheckbox">
														<label for="specialCharactersCheckbox" data-i18n="topSetVerification.specialCharacters">특수문자</label>
														<input type="text" class="txtBox minInput" id="passCombSymbol">
													</li>
												</ul>
											</div>
										</div>
									</li>
									<li class="line">
										<div class="item">
											<em data-i18n="topSetVerification.usagePeriod">사용기간</em>
											<div>
												<ul class="itemDivision">
													<li>
														<div class="selectBox w230">
															<select class="drop" name="" id="usagePeriodType">
																<option value="0" data-i18n="topSetVerification.unlimited">무기한</option>
																<option value="1" data-i18n="topSetVerification.limited">기간제한</option>
															</select>
														</div>
														<label class="hide">사용기간</label>
														<input type="text" class="txtBox minInput" id="passExpireDays">
														<span class="minTxt" data-i18n="topSetVerification.days">일</span>
													</li>
													<li>
														<input type="checkbox" name="" id="expirationNotificationCheckbox">
														<label for="expirationNotificationCheckbox" data-i18n="topSetVerification.expirationNotification">만료알림</label>
														<input type="text" class="txtBox minInput" id="passNotifyBefore">
														<span class="minTxt" data-i18n="topSetVerification.daysBefore">일전</span>
													</li>
												</ul>
											</div>
										</div>
									</li>
								</ul>
								<div class="bu_wrap">
									<button type="button" onclick="savePasswordSetting();" data-i18n="button.save">저장</button>
								</div>
							</li>
							<li class="operation">
								<div class="contDiv">
									<h3 class="contTitle" data-i18n="topSetOperation.hrIntegration">인사 연동</h3>
									<ul class="setWrap">
										<li class="line">
											<div class="item">
												<em>
													<input type="checkbox" name="" id="hrIntegrationCheckbox" ><span data-i18n="topSetOperation.hrIntegration">인사 연동</span>
												</em>
												<div>
													<ul class="itemDivision">
														<li>
															<label data-i18n="topSetOperation.operationCycle">동작 주기</label>
															<div class="selectBox" id="operationCycleBox">
																<select class="drop" name="operationCycle" id="operationCycle">
																	<option value="daily" data-i18n="topSetOperation.daily">매일</option>
																	<option value="weekly" data-i18n="topSetOperation.weekly">매주</option>
																	<option value="monthly" data-i18n="topSetOperation.monthly">매달</option>
																</select>
															</div>
															<div class="selectBox w130" id="weeklyBox">
																<select class="drop" name="weeklyDay" id="weeklyDay">
																	<option value="mon" data-i18n="topSetOperation.monday">월요일</option>
																	<option value="tue" data-i18n="topSetOperation.tuesday">화요일</option>
																	<option value="wed" data-i18n="topSetOperation.wednesday">수요일</option>
																	<option value="thu" data-i18n="topSetOperation.thursday">목요일</option>
																	<option value="fri" data-i18n="topSetOperation.friday">금요일</option>
																	<option value="sat" data-i18n="topSetOperation.saturday">토요일</option>
																	<option value="sun" data-i18n="topSetOperation.sunday">일요일</option>
																</select>
															</div>
															<div class="selectBox w130 hide" id="monthlyBox">
																<select class="drop" name="" id="monthlyDay" >
																	<option value="1">1</option>
																	<option value="2">2</option>
																	<option value="3">3</option>
																	<option value="4">4</option>
																	<option value="5">5</option>
																	<option value="6">6</option>
																	<option value="7">7</option>
																	<option value="8">8</option>
																	<option value="9">9</option>
																	<option value="10">10</option>
																	<option value="11">11</option>
																	<option value="12">12</option>
																	<option value="13">13</option>
																	<option value="14">14</option>
																	<option value="15">15</option>
																	<option value="16">16</option>
																	<option value="17">17</option>
																	<option value="18">18</option>
																	<option value="19">19</option>
																	<option value="20">20</option>
																	<option value="21">21</option>
																	<option value="22">22</option>
																	<option value="23">23</option>
																</select>
															</div>
														</li>
														<li>
															<label data-i18n="topSetOperation.integrationTime">연동 시각</label>
															<div class="selectBox w80">
																<select class="drop" name="" id="integrationHour" >
																	<option value="00">00</option>
																	<option value="01">01</option>
																	<option value="02">02</option>
																	<option value="03">03</option>
																	<option value="04">04</option>
																	<option value="05">05</option>
																	<option value="06">06</option>
																	<option value="07">07</option>
																	<option value="08">08</option>
																	<option value="09">09</option>
																	<option value="10">10</option>
																	<option value="11">11</option>
																	<option value="12">12</option>
																	<option value="13">13</option>
																	<option value="14">14</option>
																	<option value="15">15</option>
																	<option value="16">16</option>
																	<option value="17">17</option>
																	<option value="18">18</option>
																	<option value="19">19</option>
																	<option value="20">20</option>
																	<option value="21">21</option>
																	<option value="22">22</option>
																	<option value="23">23</option>
																</select>
															</div>
															<span class="mr10">:</span>
															<div class="selectBox w80">
																<select class="drop" name="" id="integrationMinute" >
																	<option value="00">00</option>
																	<option value="01">01</option>
																	<option value="02">02</option>
																	<option value="03">03</option>
																	<option value="04">04</option>
																	<option value="05">05</option>
																	<option value="06">06</option>
																	<option value="07">07</option>
																	<option value="08">08</option>
																	<option value="09">09</option>
																	<option value="10">10</option>
																	<option value="11">11</option>
																	<option value="12">12</option>
																	<option value="13">13</option>
																	<option value="14">14</option>
																	<option value="15">15</option>
																	<option value="16">16</option>
																	<option value="17">17</option>
																	<option value="18">18</option>
																	<option value="19">19</option>
																	<option value="20">20</option>
																	<option value="21">21</option>
																	<option value="22">22</option>
																	<option value="23">23</option>
																	<option value="24">24</option>
																	<option value="25">25</option>
																	<option value="26">26</option>
																	<option value="27">27</option>
																	<option value="28">28</option>
																	<option value="29">29</option>
																	<option value="30">30</option>
																	<option value="31">31</option>
																	<option value="32">32</option>
																	<option value="33">33</option>
																	<option value="34">34</option>
																	<option value="35">35</option>
																	<option value="36">36</option>
																	<option value="37">37</option>
																	<option value="38">38</option>
																	<option value="39">39</option>
																	<option value="40">40</option>
																	<option value="41">41</option>
																	<option value="42">42</option>
																	<option value="43">43</option>
																	<option value="44">44</option>
																	<option value="45">45</option>
																	<option value="46">46</option>
																	<option value="47">47</option>
																	<option value="48">48</option>
																	<option value="49">49</option>
																	<option value="50">50</option>
																	<option value="51">51</option>
																	<option value="52">52</option>
																	<option value="53">53</option>
																	<option value="54">54</option>
																	<option value="55">55</option>
																	<option value="56">56</option>
																	<option value="57">57</option>
																	<option value="58">58</option>
																	<option value="59">59</option>
																</select>
															</div>
														</li>
													</ul>
													<div class="bu_wrap">
														<button data-i18n="topSetOperation.integrationTest" id="integrationTestBtn">연동 테스트</button>
														<button data-i18n="topSetOperation.integrationExecute" id="integrationExecuteBtn">연동 실행</button>
													</div>
												</div>
											</div>
										</li>
									</ul>
								</div>
								<div class="contDiv">
									<h3 class="contTitle" data-i18n="topSetOperation.disconnectSystemDelete">미접속 시스템 삭제</h3>
									<ul class="setWrap">
										<li class="line">
											<div class="item">
												<em>
													<input type="checkbox" name="" id="disconnectSystemDeleteCheckbox" ><span data-i18n="topSetOperation.disconnectSystemDelete">미접속 시스템 삭제</span>
												</em>
												<div>
													<ul class="itemDivision">
														<li>
															<label data-i18n="topSetOperation.inactivePeriod">미접속 기간</label>
															<input type="text" class="txtBox w200" id="term">
															<span class="minTxt" data-i18n="topSetOperation.days">일</span>
														</li>
														<li>
															<label data-i18n="topSetOperation.deleteTime">삭제 시각</label>
															<div class="selectBox w80">
																<select class="drop" name="" id="deleteHour" >
																	<option value="00">00</option>
																	<option value="01">01</option>
																	<option value="02">02</option>
																	<option value="03">03</option>
																	<option value="04">04</option>
																	<option value="05">05</option>
																	<option value="06">06</option>
																	<option value="07">07</option>
																	<option value="08">08</option>
																	<option value="09">09</option>
																	<option value="10">10</option>
																	<option value="11">11</option>
																	<option value="12">12</option>
																	<option value="13">13</option>
																	<option value="14">14</option>
																	<option value="15">15</option>
																	<option value="16">16</option>
																	<option value="17">17</option>
																	<option value="18">18</option>
																	<option value="19">19</option>
																	<option value="20">20</option>
																	<option value="21">21</option>
																	<option value="22">22</option>
																	<option value="23">23</option>
																</select>
															</div>
															<span class="mr10">:</span>
															<div class="selectBox w80">
																<select class="drop" name="" id="deleteMinute" >
																	<option value="00">00</option>
																	<option value="01">01</option>
																	<option value="02">02</option>
																	<option value="03">03</option>
																	<option value="04">04</option>
																	<option value="05">05</option>
																	<option value="06">06</option>
																	<option value="07">07</option>
																	<option value="08">08</option>
																	<option value="09">09</option>
																	<option value="10">10</option>
																	<option value="11">11</option>
																	<option value="12">12</option>
																	<option value="13">13</option>
																	<option value="14">14</option>
																	<option value="15">15</option>
																	<option value="16">16</option>
																	<option value="17">17</option>
																	<option value="18">18</option>
																	<option value="19">19</option>
																	<option value="20">20</option>
																	<option value="21">21</option>
																	<option value="22">22</option>
																	<option value="23">23</option>
																	<option value="24">24</option>
																	<option value="25">25</option>
																	<option value="26">26</option>
																	<option value="27">27</option>
																	<option value="28">28</option>
																	<option value="29">29</option>
																	<option value="30">30</option>
																	<option value="31">31</option>
																	<option value="32">32</option>
																	<option value="33">33</option>
																	<option value="34">34</option>
																	<option value="35">35</option>
																	<option value="36">36</option>
																	<option value="37">37</option>
																	<option value="38">38</option>
																	<option value="39">39</option>
																	<option value="40">40</option>
																	<option value="41">41</option>
																	<option value="42">42</option>
																	<option value="43">43</option>
																	<option value="44">44</option>
																	<option value="45">45</option>
																	<option value="46">46</option>
																	<option value="47">47</option>
																	<option value="48">48</option>
																	<option value="49">49</option>
																	<option value="50">50</option>
																	<option value="51">51</option>
																	<option value="52">52</option>
																	<option value="53">53</option>
																	<option value="54">54</option>
																	<option value="55">55</option>
																	<option value="56">56</option>
																	<option value="57">57</option>
																	<option value="58">58</option>
																	<option value="59">59</option>
																</select>
															</div>
														</li>
													</ul>
													<!-- <div class="bu_wrap">
														<button type="button" data-i18n="button.viewUnconnectedSystem" id="viewUnconnectedSystemBtn" onclick="openUnusedSystemPopup();">미접속 시스템 조회</button>
													</div> -->
												</div>
											</div>
										</li>
									</ul>
								</div>
								<div class="contDiv">
									<h3 class="contTitle" data-i18n="topSetOperation.dataCleanup">데이터 정리</h3>
									<div class="tableWrap mb20">
										<table class="checkDataTable">
											<thead>
												<tr>
													<th width="5%"></th>
													<th width="27%" data-i18n="topSetOperation.grid.name">이름</th>
													<th width="43%" data-i18n="topSetOperation.grid.condition">조건</th>
													<th width="25%" data-i18n="topSetOperation.grid.cleanupTime">정리 시각</th>
												</tr>
											</thead>
											<tbody>
												<tr>
													<td><input type="checkbox" name="chk" id="operationHistoryDataDeleteCheckbox"></td>
													<td data-i18n="topSetOperation.operationHistoryDataDelete">운영 이력 데이터 삭제</td>
													<td>
														<div class="selectBox">
															<select class="drop" id="periodSelect">
																<option value="" data-i18n="topSetOperation.setPeriod">기간 지정</option>
																<option value="3" data-i18n="topSetOperation.before3Months">3개월 이전</option>
																<option value="6" data-i18n="topSetOperation.before6Months">6개월 이전</option>
																<option value="9" data-i18n="topSetOperation.before9Months">9개월 이전</option>
																<option value="12" data-i18n="topSetOperation.before12Months">12개월 이전</option>
															</select>
														</div>
														<input type="text" class="datepicker txtBox" id="startDatePicker" readonly data-i18n-placeholder="datepicker.startDate">
														<input type="text" class="datepicker txtBox" id="endDatePicker" readonly data-i18n-placeholder="datepicker.endDate">
													</td>
													<td>
														<div class="selectBox w80">
															<select class="drop" name="" id="operationHour" >
																<option value="00">00</option>
																<option value="01">01</option>
																<option value="02">02</option>
																<option value="03">03</option>
																<option value="04">04</option>
																<option value="05">05</option>
																<option value="06">06</option>
																<option value="07">07</option>
																<option value="08">08</option>
																<option value="09">09</option>
																<option value="10">10</option>
																<option value="11">11</option>
																<option value="12">12</option>
																<option value="13">13</option>
																<option value="14">14</option>
																<option value="15">15</option>
																<option value="16">16</option>
																<option value="17">17</option>
																<option value="18">18</option>
																<option value="19">19</option>
																<option value="20">20</option>
																<option value="21">21</option>
																<option value="22">22</option>
																<option value="23">23</option>
															</select>
														</div>
														<span class="mr10">:</span>
														<div class="selectBox w80">
															<select class="drop" name="" id="operationMinute" >
																<option value="00">00</option>
																<option value="01">01</option>
																<option value="02">02</option>
																<option value="03">03</option>
																<option value="04">04</option>
																<option value="05">05</option>
																<option value="06">06</option>
																<option value="07">07</option>
																<option value="08">08</option>
																<option value="09">09</option>
																<option value="10">10</option>
																<option value="11">11</option>
																<option value="12">12</option>
																<option value="13">13</option>
																<option value="14">14</option>
																<option value="15">15</option>
																<option value="16">16</option>
																<option value="17">17</option>
																<option value="18">18</option>
																<option value="19">19</option>
																<option value="20">20</option>
																<option value="21">21</option>
																<option value="22">22</option>
																<option value="23">23</option>
																<option value="24">24</option>
																<option value="25">25</option>
																<option value="26">26</option>
																<option value="27">27</option>
																<option value="28">28</option>
																<option value="29">29</option>
																<option value="30">30</option>
																<option value="31">31</option>
																<option value="32">32</option>
																<option value="33">33</option>
																<option value="34">34</option>
																<option value="35">35</option>
																<option value="36">36</option>
																<option value="37">37</option>
																<option value="38">38</option>
																<option value="39">39</option>
																<option value="40">40</option>
																<option value="41">41</option>
																<option value="42">42</option>
																<option value="43">43</option>
																<option value="44">44</option>
																<option value="45">45</option>
																<option value="46">46</option>
																<option value="47">47</option>
																<option value="48">48</option>
																<option value="49">49</option>
																<option value="50">50</option>
																<option value="51">51</option>
																<option value="52">52</option>
																<option value="53">53</option>
																<option value="54">54</option>
																<option value="55">55</option>
																<option value="56">56</option>
																<option value="57">57</option>
																<option value="58">58</option>
																<option value="59">59</option>
															</select>
														</div>
													</td>
												</tr>
												<tr>
													<td><input type="checkbox" name="chk" id="productHistoryDataDeleteCheckbox"></td>
													<td data-i18n="topSetOperation.productHistoryDataDelete">제품 이력 데이터 삭제</td>
													<td>
														<div class="selectBox">
															<select class="drop dateSelectSetData" name="" id="periodSelect2">
																<option value="" data-i18n="topSetOperation.setPeriod">기간 지정</option>
																<option value="3" data-i18n="topSetOperation.before3Months">3개월 이전</option>
																<option value="6" data-i18n="topSetOperation.before6Months">6개월 이전</option>
																<option value="9" data-i18n="topSetOperation.before9Months">9개월 이전</option>
																<option value="12" data-i18n="topSetOperation.before12Months">12개월 이전</option>
															</select>
														</div>
														<input class="datepicker datePickerSetData txtBox" data-i18n-placeholder="datepicker.startDate" readonly id="startDatePicker2" />
														<input class="datepicker datePickerSetData txtBox" data-i18n-placeholder="datepicker.endDate" readonly id="endDatePicker2" />
													</td>
													<td>
														<div class="selectBox w80">
															<select class="drop" name="" id="productHour" >
																<option value="00">00</option>
																<option value="01">01</option>
																<option value="02">02</option>
																<option value="03">03</option>
																<option value="04">04</option>
																<option value="05">05</option>
																<option value="06">06</option>
																<option value="07">07</option>
																<option value="08">08</option>
																<option value="09">09</option>
																<option value="10">10</option>
																<option value="11">11</option>
																<option value="12">12</option>
																<option value="13">13</option>
																<option value="14">14</option>
																<option value="15">15</option>
																<option value="16">16</option>
																<option value="17">17</option>
																<option value="18">18</option>
																<option value="19">19</option>
																<option value="20">20</option>
																<option value="21">21</option>
																<option value="22">22</option>
																<option value="23">23</option>
															</select>
														</div>
														<span class="mr10">:</span>
														<div class="selectBox w80">
															<select class="drop" name="" id="productMinute" >
																<option value="00">00</option>
																<option value="01">01</option>
																<option value="02">02</option>
																<option value="03">03</option>
																<option value="04">04</option>
																<option value="05">05</option>
																<option value="06">06</option>
																<option value="07">07</option>
																<option value="08">08</option>
																<option value="09">09</option>
																<option value="10">10</option>
																<option value="11">11</option>
																<option value="12">12</option>
																<option value="13">13</option>
																<option value="14">14</option>
																<option value="15">15</option>
																<option value="16">16</option>
																<option value="17">17</option>
																<option value="18">18</option>
																<option value="19">19</option>
																<option value="20">20</option>
																<option value="21">21</option>
																<option value="22">22</option>
																<option value="23">23</option>
																<option value="24">24</option>
																<option value="25">25</option>
																<option value="26">26</option>
																<option value="27">27</option>
																<option value="28">28</option>
																<option value="29">29</option>
																<option value="30">30</option>
																<option value="31">31</option>
																<option value="32">32</option>
																<option value="33">33</option>
																<option value="34">34</option>
																<option value="35">35</option>
																<option value="36">36</option>
																<option value="37">37</option>
																<option value="38">38</option>
																<option value="39">39</option>
																<option value="40">40</option>
																<option value="41">41</option>
																<option value="42">42</option>
																<option value="43">43</option>
																<option value="44">44</option>
																<option value="45">45</option>
																<option value="46">46</option>
																<option value="47">47</option>
																<option value="48">48</option>
																<option value="49">49</option>
																<option value="50">50</option>
																<option value="51">51</option>
																<option value="52">52</option>
																<option value="53">53</option>
																<option value="54">54</option>
																<option value="55">55</option>
																<option value="56">56</option>
																<option value="57">57</option>
																<option value="58">58</option>
																<option value="59">59</option>
															</select>
														</div>
													</td>
												</tr>
											</tbody>
										</table>
									</div>
								</div>
								<div class="contDiv">
									<h3 class="contTitle" data-i18n="topSetOperation.agentAccess">Agent 접속</h3>
									<ul class="setWrap">
										<li class="line">
											<div class="item">
												<em>
													<span data-i18n="topSetOperation.agentAccess">Agent 접속</span>
												</em>
												<div>
													<ul class="itemDivision">
														<li>
															<label data-i18n="topSetOperation.keepAliveCycle">Keep Alive 주기</label>
															<input type="text" class="txtBox w200" id="keepalive">
															<span class="minTxt" data-i18n="topSetOperation.minutes">분</span>
														</li>
														<li>
															<label data-i18n="topSetOperation.logSendCycle">로그 전송 주기</label>
															<input type="text" class="txtBox w200" id="logPush">
															<span class="minTxt" data-i18n="topSetOperation.minutes">분</span>
														</li>
														<!-- <li>
															<label data-i18n="topSetOperation.connectionStatusChangeTime">접속상태 변경 시간</label>
															<input type="text" class="txtBox w200" id="statePush">
															<span class="minTxt" data-i18n="topSetOperation.minutes">분</span>
					
														</li> -->
													</ul>
												</div>
											</div>
										</li>
									</ul>
								</div>
								<div class="bu_wrap">
									<button type="button" data-i18n="button.save" onclick="saveServerConfig();">저장</button>
								</div>
							</li>
						</ul>
					</div>
					<div class="bu_wrap">
						<button type="button" data-i18n="button.close" class="lineBtn" onclick="$('.topSetPopup').addClass('hide');">닫기</button>
					</div>
				</div>
			</form>
			<button class="bu_close_pop"></button>
		</div>
	</section>

	<!-- 상단 설정 팝업 > 운영 > 미접속 시스템 삭제 -->
	<section class="popup deleteUnconnectedSystemPopup hide">
		<div class="bgscreen"></div>
		<div class="pop_wrap">
			<strong class="pop_head" data-i18n="topSetOperation.deleteUnconnectedSystemPopup.popTitle">미접속 시스템 삭제</strong>
			<form action="">
				<div class="pop_cont">
					<div class="tableWrap">
						<table class="dataTable" id="deleteUnconnectedSystemTable" style="width:100%">
							<thead>
								<tr>
									<th width="5%" data-i18n="topSetOperation.deleteUnconnectedSystemPopup.grid.select">선택</th>
									<th width="10%" data-i18n="topSetOperation.deleteUnconnectedSystemPopup.grid.company">회사</th>
									<th data-i18n="topSetOperation.deleteUnconnectedSystemPopup.grid.department">부서</th>
									<th width="20%" data-i18n="topSetOperation.deleteUnconnectedSystemPopup.grid.name">사용자명</th>
									<th width="15%" data-i18n="topSetOperation.deleteUnconnectedSystemPopup.grid.lastLoginDate">마지막 접속일</th>
								</tr>
							</thead>
							<tbody>
								<tr>
									<td></td>
									<td>회사</td>
									<td>영업부</td>
									<td>홍길동</td>
									<td>2024-10-01</td>
								</tr>
								<tr>
									<td></td>
									<td>회사</td>
									<td>영업부2</td>
									<td>홍길동2</td>
									<td>2024-10-01</td>
								</tr>
							</tbody>
						</table>
					</div>
					<div class="bu_wrap">
						<button type="button" class="saveBtn" data-i18n="button.delete" onclick="deleteUnusedSystem();">삭제</button>
						<button type="button" class="lineBtn" data-i18n="button.close" onclick="closeUnusedSystemPopup();">닫기</button>
					</div>
				</div>
			</form>
			<button class="bu_close_pop"></button>
		</div>
	</section>

	<!-- 공통 사용 팝업 : 상단 설정 아이콘 팝업 > 라이선스 > 라이선스 등록-->
	<section class="popup licenseRegiPopup hide">
		<div class="bgscreen"></div>
		<div class="pop_wrap">
			<strong class="pop_head" data-i18n="licenseRegiPopup.popTitle">라이선스 등록</strong>
			<form action="">
				<div class="pop_cont">
					<div class="inputFileWrap mb10">
						<input class="inputFile" id="upload1" type="file" name="encryptedFile" accept=".dat" multiple>
						<span class="spanFile" id="spanFile1" data-i18n="licenseRegiPopup.licenseEncryptionFile">라이선스 암호화 파일</span>
						<label class="labelFile w150" for="upload1" data-i18n="button.fileSelection">파일선택</label>
					</div>
					<div class="inputFileWrap">
						<input class="inputFile" id="upload2" type="file" name="keyFile" accept=".ini" multiple>
						<span class="spanFile" id="spanFile2" data-i18n="licenseRegiPopup.decryptionKeyFile">암호화 해제 키 파일</span>
						<label class="labelFile w150" for="upload2" data-i18n="button.fileSelection">파일선택</label>
					</div>
					<div class="bu_wrap">
						<button type="button" class="saveBtn" onclick="saveLicense();" data-i18n="button.register">등록</button>
						<button type="button" class="lineBtn" onclick="closeLicensePopup();" data-i18n="button.cancel">취소</button>
					</div>
				</div>
			</form>
			<button class="bu_close_pop"></button>
		</div>
	</section>
	
	<!-- 프로필 팝업 -->
	<section class="popup profilePopup hide">
		<div class="bgscreen"></div>
		<div class="pop_wrap">
			<strong class="pop_head" data-i18n="topProfile.title">프로필</strong>
			<form action="">
				<div class="pop_cont">
					<ul class="setWrap">
						<li class="line">
							<div class="item">
								<em data-i18n="topProfile.id">아이디</em>
								<!-- nowrite 클래스 제거하면 텍스트 입력 가능한 모양으로 변함 -->
								<div><input type="text" class="txtBox nowrite" id="profilePopupUserId" readonly></div>
							</div>
						</li>
						<li class="line">
							<div class="item">
								<em data-i18n="topProfile.name">이름</em>
								<div><input type="text" class="txtBox nowrite modifyYes" id="profilePopupUserName" readonly></div>
							</div>
						</li>
						<li class="line">
							<div class="item">
								<em data-i18n="topProfile.email">이메일</em>
								<div><input type="text" class="txtBox nowrite modifyYes" id="profilePopupUserEmail" readonly></div>
							</div>
						</li>
						<li class="line">
							<div class="item">
								<em data-i18n="topProfile.permission">권한</em>
								<div><input type="text" class="txtBox nowrite" id="profilePopupUserRole" readonly></div>
							</div>
						</li>
					</ul>
					<ul class="setWrap">
						<li class="line">
							<div class="item">
								<em>
									<span data-i18n="topProfile.accountDate">계정 만료일</span>
									<span class="questionMark">
										<span class="questionTxt" style="width: 500px;" data-i18n="topProfile.accountText">계정 사용기간 만료 전 '신청 및 결재 > 신청' 페이지에서 계정 연장 신청을 하세요.</span>
									</span>
								</em>
								<div class="flexWrap">
									<input type="text" class="txtBox nowrite mr00" id="profilePopupUserExpireDate" readonly>
									<div class="bu_wrap w310">
										<button type="button" id="accountExtensionBtn" class="accountExtensionBtn hide" onclick="openAccountExtensionPopup();" data-i18n="button.accountExtension">계정 연장 신청</button>
									</div>
								</div>
							</div>
						</li>
						<li class="line">
							<div class="item">
								<em data-i18n="topProfile.pwDate">비밀번호 만료일</em>
								<div><input type="text" class="txtBox nowrite" id="profilePopupUserPasswordExpireDate" readonly></div>
							</div>
						</li>
					</ul>
					<div class="bu_wrap">
						<button class="changePasswordBtn" type="button" onclick="initChangePasswordPopup();" data-i18n="button.pwChange">비밀번호 변경</button>
						<button class="modifyProfileBtn" type="button" data-i18n="button.edit">수정</button>
						<button class="saveBtn hide" type="button" onclick="saveProfile();" data-i18n="button.save">저장</button>
						<button class="lineBtn" type="button" data-i18n="button.close" onclick="$('.profilePopup').addClass('hide');">닫기</button>
					</div>
				</div>
			</form>
			<button class="bu_close_pop"></button>
		</div>
	</section>
	
	<!-- popup -->
	<!-- 결재 신청 -->
	<section class="popup createRequestPopup innerVerPop hide">
		<div class="bgscreen"></div>
		<div class="pop_wrap">
			<strong class="pop_head" data-i18n="createRequest.createRequestPopup.popTitle">신청</strong>
			<form action="">
				<div class="pop_cont">
					<ul class="lineWrap">
					</ul>
					<div class="flexWrap">
						<div class="selectBox typeOfApplication wAuto">
							<select class="drop" name="requestApprovalCode" id="requestApprovalCode">
								<option value="M" data-i18n="createRequest.createRequestPopup.createMoveFile">서식 : 이동파일 생성 신청</option>
								<option value="C" data-i18n="createRequest.createRequestPopup.accountExtension">서식 : 계정 연장 신청</option>
								<option value="A" data-i18n="createRequest.createRequestPopup.userRegistration">서식 : 사용자 등록 신청</option>
							</select>
						</div>
						<div class="bu_wrap flexWrap alignCenter" id="moveFielViewBtn">
							<span class="questionMark mr10">
								<span class="questionTxt" style="width: 483px;" data-i18n="createRequest.createRequestPopup.moveFileInfo">'이동파일 조회' 버튼을 클릭하여 생성된 이동파일 정보를 선택한 후 신청하세요.</span>
							</span>
							<button type="button" class="moveFileListSearchBtn" onclick="openMoveFileListSearchPopup();" data-i18n="button.moveFileInquiryBtn">이동파일 조회</button>
						</div>
					</div>
					<ul class="setWrap mt15">
						<li class="line w2">
							<div class="item">
								<em data-i18n="createRequest.createRequestPopup.requestno">신청 번호</em>
								<div class="documentNumber" id="requestCode">자동채번</div>
							</div>
							<div class="item">
								<em data-i18n="createRequest.createRequestPopup.requestDate">신청일자</em>
								<div class="date" id="requestDate"></div>
							</div>
						</li>
						<li class="line">
							<div class="item">
								<em data-i18n="createRequest.createRequestPopup.submitter">신청자</em>
								<div id="requestUserNm"></div>
								<input type="hidden" id="requestUserId">
							</div>
						</li>
						<li class="line">
							<div class="item">
								<em data-i18n="createRequest.createRequestPopup.title">제목</em>
								<div>
									<input type="text" class="txtBox" id="requestTitle">
								</div>
							</div>
						</li>
					</ul>
					<div class="approvalFormTypeM">
						<div class="txtWrap">
						<h3 class="title" data-i18n="createRequest.createRequestPopup.noti">공지</h3>
							<textarea name="" id="createRequestPopupNoticeM" readonly></textarea>
						</div>
						<div class="txtWrap">
							<h3 class="title" data-i18n="createRequest.createRequestPopup.registeredFile">등록파일</h3>
							<div class="upFileList" id="viewFiles"></div>
						</div>
						<div class="txtWrap mb00">
							<h3 class="title" data-i18n="createRequest.createRequestPopup.reason">사유</h3>
							<textarea name="" class="" id="requestReasonM"></textarea>
						</div>
					</div>
					<div class="approvalFormTypeC hide">
						<div class="txtWrap">
							<h3 class="title" data-i18n="createRequest.createRequestPopup.noti">공지</h3>
							<textarea name="" id="createRequestPopupNoticeC" readonly></textarea>
						</div>
						<div class="txtWrap">
							<h3 class="title" data-i18n="createRequest.createRequestPopup.accountExtensionPe">계정 연장 기간</h3>
							<div class="selectBox">
								<select class="drop dateSelectSetData2" name="accountExtensionPeriod" id="accountExtensionPeriod">
									<option value="0" data-i18n="createRequest.createRequestPopup.specifyPeriod">기간 지정</option>
									<option value="3" data-i18n="createRequest.createRequestPopup.3month">3개월</option>
									<option value="6" data-i18n="createRequest.createRequestPopup.6month">6개월</option>
									<option value="9" data-i18n="createRequest.createRequestPopup.9month">9개월</option>
									<option value="12" data-i18n="createRequest.createRequestPopup.12month">12개월</option>
								</select>
							</div>
							<input class="datepicker datePickerSetDataEnd txtBox" data-i18n-placeholder="datepicker.endDate" readonly id="accountExtensionPeriodEnd" />
						</div>
						<div class="txtWrap mb00">
							<h3 class="title" data-i18n="createRequest.createRequestPopup.reason">사유</h3>
							<textarea name="" class="" id="requestReasonC"></textarea>
						</div>
					</div>
					<div class="approvalFormTypeA hide">
						<div class="txtWrap">
							<h3 class="title" data-i18n="createRequest.createRequestPopup.noti">공지</h3>
							<textarea name="" id="createRequestPopupNoticeA" readonly></textarea>
						</div>
						<div class="txtWrap">
							<h3 class="title" data-i18n="createRequest.createRequestPopup.userInfo">등록할 사용자 정보</h3>
							<ul class="setWrap">
								<li class="line">
									<div class="item">
										<em data-i18n="createRequest.createRequestPopup.userId">사용자 ID</em>
										<div class="idCheck">
											<input type="text" class="txtBox" name="userId" placeholder="" id="userId">
											<div class="bu_wrap">
												<button class="w160" type="button" onclick="checkUserId($('#userId').val());" data-i18n="button.duplicateCheck">중복 체크</button>
												<input type="hidden" name="idCheckFlag" value="N">
												<input type="hidden" name="checkedUserId" value="">
											</div>
										</div>
									</div>
								</li>
								<li class="line">
									<div class="item">
										<em data-i18n="createRequest.createRequestPopup.userName">사용자명</em>
										<div class="">
											<input type="text" class="txtBox" name="userName" id="userName">
										</div>
									</div>
								</li>
								<li class="line">
									<div class="item">
										<em data-i18n="createRequest.createRequestPopup.email">이메일</em>
										<div class="idCheck">
											<input type="text" class="txtBox" name="email" id="email">
											<div class="bu_wrap">
												<button class="w160" type="button" onclick="checkMail($('#email').val());" data-i18n="button.duplicateCheck">중복 체크</button>
												<input type="hidden" name="mailCheckFlag" value="N">
												<input type="hidden" name="checkedMail" value="">
											</div>
										</div>
									</div>
								</li>
								<!-- <li class="line hide">
									<div class="item">
										<em>회사</em>
										<div class="adminLevel">
											<div class="selectBox">
												<select class="drop" name="companyId" id="companyId">
													<option value="ROLE_PRIMX_ADMIN" >회사A</option>
													<option value="ROLE_PRIMX_USER">회사B</option>
												</select>
											</div>
										</div>
									</div>
								</li> -->
								<li class="line">
									<div class="item">
										<em data-i18n="createRequest.createRequestPopup.department">부서</em>
										<div class="idCheck">
											<input type="text" class="txtBox" name="deptName" id="deptName" readonly>
											<input type="hidden" name="companyId" id="companyId">
											<input type="hidden" name="deptId" id="deptId">
											<input type="hidden" name="isDeptIdValid" id="isDeptIdValid">
											<div class="bu_wrap">
												<button class="departSelectBtn w160" type="button" onclick="openDepartSelectPopup(null, null);" data-i18n="button.selectDepart">부서 선택</button>
											</div>
										</div>
									</div>
								</li>
								<li class="line">
									<div class="item">
										<em data-i18n="createRequest.createRequestPopup.userLevel">사용자 레벨</em>
										<div class="adminLevel">
											<div class="selectBox">
												<select class="drop" name="userRole" id="userRole">
													<!-- <option value="ROLE_PRIMX_ADMIN">최고 관리자</option>
													<option value="ROLE_PRIMX_USER">일반 관리자</option>
													<option value="ROLE_PRIMX_USER">사용자</option>
													<option value="ROLE_PRIMX_USER">공용 PC</option> -->
												</select>
											</div>
										</div>
									</div>
								</li>
								<li class="line">
									<div class="item">
										<em data-i18n="createRequest.createRequestPopup.usePeriod">사용기간</em>
										<div class="useDate">
											<div class="selectBox">
												<select class="drop" name="userRegistrationPeriod" id="userRegistrationPeriod">
													<option value="0" data-i18n="createRequest.createRequestPopup.unlimited">무기한</option>
													<option value="1" data-i18n="createRequest.createRequestPopup.specifyPeriod">기간 지정</option>
												</select>
											</div>
											<input class="datepicker txtBox" name="userRegistrationExpiredDate" data-i18n-placeholder="datepicker.endDate" readonly id="userRegistrationExpiredDate" />
										</div>
									</div>
								</li>
							</ul>
						</div>
						<div class="txtWrap mb00">
							<h3 class="title" data-i18n="createRequest.createRequestPopup.reason">사유</h3>
							<textarea name="" id="requestReasonA"></textarea>
						</div>
					</div>
					<div class="bu_wrap">
						<!-- <button type="button" class="line registrationApprovalBtn">결재선 등록</button> -->
						<button class="save" type="button" onclick="saveRequest();" data-i18n="button.requestApproval">결재요청</button>
						<button class="lineBtn" type="button" data-i18n="button.close" onclick="$('.createRequestPopup').addClass('hide');">닫기</button>
					</div>
				</div>
			</form>
			<button class="bu_close_pop"></button>
		</div>
	</section>
	
	<!-- 결재선 지정 -->
	<section class="popup approvalListPopup innerVerPop hide">
		<div class="bgscreen"></div>
		<div class="pop_wrap">
			<strong class="pop_head" data-i18n="createRequest.approvalListPopup.popTitle">결재선 지정</strong>
			<form action="">
				<div class="pop_cont">
					<ul class="lineWrap">
					</ul>
					<div class="departUserSection">
						<div class="departmentArea">
							<div class="searchInnerWrap_p mb15">
								<div class="searchBox">
									<input type="text" class="txtBox searchInput" readonly data-i18n-placeholder="search.departmentSearch" onclick="initDeptSearchPopup('.searchArea_p_Popup.departSearchPopup');">
									<!-- <p class="filterItem">
										<span class="filterTitle">검색어</span>
										<span> : </span>
										<span class="filterSelect">영업부</span>
										<span class="delBtn"></span>
									</p> -->
								</div>
							</div>
							<div class="departTreeMap" id="deptTreeContainer">
								<!-- 트리가 여기에 렌더링됨 -->
							</div>
						</div>
						<div class="userArea">
							<div class="searchInnerWrap_p mb15">
								<div class="searchBox">
									<input type="text" class="txtBox searchInput" readonly data-i18n-placeholder="search.userSearch" onclick="initUserSearchPopup('.searchArea_p_Popup.userSearchPopup');">
									<!-- <p class="filterItem">
										<span class="filterTitle">검색어</span>
										<span> : </span>
										<span class="filterSelect">영업부</span>
										<span class="delBtn"></span>
									</p> -->
									<div class="checkWrap">
											<input type="checkbox" name="includeSubDept" id="includeSubDept" class="mr10">
											<label data-i18n="search.includeSub">하위 부서 포함</label>
										</div>
								</div>
							</div>
							<div class="userBottom">
								<div class="tableWrap">
									<table id="deptUserTable">
										<thead>
											<tr>
												<th width="30%" data-i18n="createRequest.approvalListPopup.grid.department">부서</th>
												<th width="30%" data-i18n="createRequest.approvalListPopup.grid.userName">사용자명</th>
												<th width="40%" data-i18n="createRequest.approvalListPopup.grid.userId">사용자 ID</th>
											</tr>
										</thead>
										<tbody>
										</tbody>
									</table>
								</div>
							</div>
						</div>
					</div>
					
					<div class="bu_wrap">
						<button type="button" onclick="applyApprovalLine();" data-i18n="button.applyApprovalLine">결재선 적용</button>
						<button type="button" class="lineBtn" onclick="closeApprovalListPopup();" data-i18n="button.close">닫기</button>
					</div>
				</div>
			</form>
			<button class="bu_close_pop"></button>
		</div>
	</section>
	
	<!-- 비밀번호 변경 팝업 -->
	<section class="popup changePasswordPopup hide">
		<div class="bgscreen"></div>
		<div class="pop_wrap">
			<strong class="pop_head" data-i18n="changePasswordPopup.title">비밀번호 변경</strong>
			<form action="">
				<div class="pop_cont">
					<ul>
						<li>
							<input type="password" data-i18n-placeholder="changePasswordPopup.currentPassword" class="txtBox pwInput" id="changePasswordPopupCurrentPassword"/>
							<button type="button" class="pwViewBtn" onclick="passwordEyes();"></button>
						</li>
						<li>
							<input type="password" data-i18n-placeholder="changePasswordPopup.newPassword" class="txtBox pwInput" id="changePasswordPopupNewPassword"/>
							<button type="button" class="pwViewBtn" onclick="passwordEyes();"></button>
						</li>
						<li>
							<input type="password" data-i18n-placeholder="changePasswordPopup.confirmPassword" class="txtBox pwInput" id="changePasswordPopupNewPasswordConfirm"/>
							<button type="button" class="pwViewBtn" onclick="passwordEyes();"></button>
						</li>
					</ul>
					<div class="bu_wrap">
						<button class="" type="button" onclick="updatePassword();" data-i18n="button.confirm">확인</button>
						<button class="lineBtn" type="button" onclick="$('.changePasswordPopup').addClass('hide');"data-i18n="button.cancel">취소</button>
					</div>
				</div>
			</form>
			<button class="bu_close_pop"></button>
		</div>
	</section>
	
	
	<!-- 검색 팝업(부서) -->
	<section class="popup searchArea_p_Popup departSearchPopup hide">
		<div class="bgscreen"></div>
		<div class="pop_wrap">
			<strong class="pop_head" data-i18n="search.searchTit">검색</strong>
			<form action="">
				<div class="pop_cont">
					<ul class="search_wrap">
						<li class="line">
							<div class="item">
								<label data-i18n="search.searchSubjects.department">부서</label>
								<div>
									<div class="flexWrap sbStart">
										<input type="text" id="commonDeptSearchKeyword" class="txtBox w100Per" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchApprovalLine('searchArea_p_Popup departSearchPopup');}">
									</div>
								</div>
							</div>
						</li>
					</ul>
					<div class="bu_wrap">
						<button type="button" onclick="searchApprovalLine('searchArea_p_Popup departSearchPopup');" data-i18n="button.search">검색</button>
						<button type="button" class="lineBtn" onclick="$('.departSearchPopup').addClass('hide');" data-i18n="button.close">닫기</button>
					</div>
				</div>
			</form>
			<button class="bu_close_pop"></button>
		</div>
	</section>

	<!-- 검색 팝업(사용자) -->
	<section class="popup searchArea_p_Popup userSearchPopup hide">
		<div class="bgscreen"></div>
		<div class="pop_wrap">
			<strong class="pop_head" data-i18n="search.searchTit">검색</strong>
			<form action="">
				<div class="pop_cont">
					<ul class="search_wrap">
						<li class="line">
							<div class="item">
								<label data-i18n="search.searchSubjects.user">사용자</label>
								<div class="selectBox">
									<select class="drop" name="" id="commonUserSelectOption">
										<option value="" data-i18n="search.option.all">전체</option>
										<option value="admin" data-i18n="search.option.admin">관리자</option>
										<option value="user" data-i18n="search.option.user">사용자</option>
									</select>
								</div>
								<input type="text" class="txtBox w330" id="selectedUserName" onkeypress="if(event.keyCode==13) {event.preventDefault(); searchApprovalLine('searchArea_p_Popup userSearchPopup');}">
							</div>
						</li>
						<li class="line">
							<div class="item">
								<label data-i18n="search.searchSubjects.type">구분</label>
								<div class="selectBox">
									<select class="drop" name="" id="commonHrSelectOption">
										<option value="" data-i18n="search.option.all">전체</option>
										<option value="Y" data-i18n="search.option.integration">연동</option>
										<option value="N" data-i18n="search.option.manual">수동</option>
									</select>
								</div>
								<input type="text" class="txtBox w330 hide" id="userSearchKeyword">
							</div>
						</li>
					</ul>

					<div class="bu_wrap">
						<button type="button" onclick="searchApprovalLine('searchArea_p_Popup userSearchPopup');" data-i18n="button.search">검색</button>
						<button type="button" class="lineBtn" onclick="$('.userSearchPopup').addClass('hide');" data-i18n="button.close">닫기</button>
					</div>
				</div>
			</form>

			<button class="bu_close_pop"></button>
		</div>
	</section>

	<!-- XZED 스마트 컨트랙트 설정 -->
	<section class="popup xzedSmartContractSettingPopup hide">
		<div class="bgscreen"></div>
		<div class="pop_wrap">
			<strong class="pop_head" data-i18n="xzedSmartContractSettingPopup.popTitle">XZED 스마트 컨트랙트 설정</strong>
			<form action="">
				<div class="pop_cont">
					<div class="smartDivision">
						<div class="topArea">
							<div class="departTreeMap">
								<!-- 트리가 여기에 렌더링됨 -->
							</div>
							<div class="contWrap">
								<h3 class="contTitle" data-i18n="xzedSmartContractSettingPopup.companyInfo">협력사 정보</h3>
								<div class="tableWrap">
									<table class="dataTable" id="smartContractUserTable">
										<thead>
											<tr>
												<th data-i18n="xzedSmartContractSettingPopup.grid.companyName">회사명</th>
												<th data-i18n="xzedSmartContractSettingPopup.grid.name">성명</th>
												<th data-i18n="xzedSmartContractSettingPopup.grid.email">메일계정</th>
												<th>AgentID</th>
											</tr>
										</thead>
										<tbody>
										</tbody>
									</table>
								</div>
							</div>
						</div>
						<div class="bottomArea">
							<div class="smartContractFileList">
								<h3 class="contTitle" data-i18n="xzedSmartContractSettingPopup.smartContractFileSelect">스마트 컨트랙트 파일 선택</h3>
								<div class="tableWrap">
									<table class="dataTable" id="smartContractFileListTable">
										<thead>
											<tr>
												<th data-i18n="xzedSmartContractSettingPopup.grid.select">선택</th>
												<th data-i18n="xzedSmartContractSettingPopup.grid.file">파일명</th>
												<th data-i18n="xzedSmartContractSettingPopup.grid.applicant">신청자</th>
											</tr>
										</thead>
										<tbody>
											<tr>
												<td><input type="checkbox" name="rowSelect" class="row-checkbox" /></td>
												<td>파일명넣기</td>
												<td>신청자명넣기</td>
											</tr>
											<tr>
												<td><input type="checkbox" name="rowSelect" class="row-checkbox" /></td>
												<td>파일명넣기</td>
												<td>신청자명넣기</td>
											</tr>
											<tr>
												<td><input type="checkbox" name="rowSelect" class="row-checkbox" /></td>
												<td>파일명넣기</td>
												<td>신청자명넣기</td>
											</tr>
											<tr>
												<td><input type="checkbox" name="rowSelect" class="row-checkbox" /></td>
												<td>파일명넣기</td>
												<td>신청자명넣기</td>
											</tr>
											<tr>
												<td><input type="checkbox" name="rowSelect" class="row-checkbox" /></td>
												<td>파일명넣기</td>
												<td>신청자명넣기</td>
											</tr>
											<tr>
												<td><input type="checkbox" name="rowSelect" class="row-checkbox" /></td>
												<td>파일명넣기</td>
												<td>신청자명넣기</td>
											</tr>
											<tr>
												<td><input type="checkbox" name="rowSelect" class="row-checkbox" /></td>
												<td>파일명넣기</td>
												<td>신청자명넣기</td>
											</tr>
										</tbody>
									</table>
								</div>
							</div>
							<div class="contWrap">
								<div class="flexWrap sbStart alignCenter">
									<h3 class="contTitle mr10" data-i18n="xzedSmartContractSettingPopup.smartContractTarget">스마트 컨트랙트 대상자</h3>
									<div class="selectBox wAuto">
										<select class="drop" id="xzedSmartContractOptionSelect">
											<option value="" data-i18n="xzedSmartContractSettingPopup.smartContractOption">스마트 컨트랙트 옵션 선택</option>
											<option value="" data-i18n="xzedSmartContractSettingPopup.userIdentification">사용자 식별</option>
											<option value="" data-i18n="xzedSmartContractSettingPopup.readWriteSave">읽기/쓰기/저장</option>
											<option value="" data-i18n="xzedSmartContractSettingPopup.hashOption">해시 옵션</option>
										</select>
									</div>
								</div>
								<div class="tableWrap">
									<table class="dataTable" id="xzedSmartContractSettingTable">
										<thead>
											<tr>
												<th data-i18n="xzedSmartContractSettingPopup.grid.companyName">회사명</th>
												<th data-i18n="xzedSmartContractSettingPopup.grid.name">성명</th>
												<th data-i18n="xzedSmartContractSettingPopup.grid.email">메일계정</th>
												<th data-i18n="xzedSmartContractSettingPopup.grid.phone">전화번호</th>
												<th data-i18n="xzedSmartContractSettingPopup.grid.period" class="periodTh">기간</th>
												<th>IP</th>
												<th>MAC</th>
												<th data-i18n="xzedSmartContractSettingPopup.grid.delete">삭제</th>
											</tr>
										</thead>
										<tbody>
											<tr>
												<td>회사명</td>
												<td>성명</td>
												<td>로그인계정넣기기</td>
												<td>010-2222-3333</td>
												<td>
													<div class="selectBox wAuto">
														<select class="drop" id="xzedPeriodSelect">
															<option value="" data-i18n="xzedSmartContractSettingPopup.setPeriod">기간 지정</option>
															<option value="3" data-i18n="xzedSmartContractSettingPopup.3month">3개월</option>
															<option value="6" data-i18n="xzedSmartContractSettingPopup.6month">6개월</option>
															<option value="12" data-i18n="xzedSmartContractSettingPopup.12month">12개월</option>
															<option value="0" data-i18n="xzedSmartContractSettingPopup.unlimited">무기한</option>
														</select>
													</div>
													<input type="text" class="datepicker txtBox" id="xzedStartDate" readonly data-i18n-placeholder="datepicker.startDate">
													<input type="text" class="datepicker txtBox" id="xzedEndDate" readonly data-i18n-placeholder="datepicker.endDate">
												</td>
												<td>192.168.1.90</td>
												<td>8C-3B-4A-5A-B7-28</td>
												<td>
													<div class="bu_wrap mr10">
														<button class="" type="button" data-i18n="button.delete">삭제</button>
													</div>
												</td>
											</tr>
										</tbody>
									</table>
								</div>
							</div>
						</div>
					</div>
					<div class="bu_wrap">
						<button type="button" data-i18n="button.apply" onclick="applyXZEDSmartContractSetting();">적용</button>
						<button type="button" class="lineBtn" onclick="closeXZEDSmartContractSettingPopup();" data-i18n="button.close">닫기</button>
					</div>
				</div>
			</form>
			<button class="bu_close_pop"></button>
		</div>
	</section>

	<!-- 공통 로딩 화면 -->
	<section class="popup loadingPopup hide" id="loadingPopup">
	    <div class="loader">
			<svg class="circular">
			<circle class="path" cx="50" cy="50" r="20" fill="none" stroke-width="5" stroke-miterlimit="10"></circle>
			</svg>
		</div>
	</section>
</body>
</html>

<script>
    var contextPath = "${pageContext.request.contextPath}";
    
    // 프로필 팝업 호출 로직
    $(document).ready(function() {
        <c:if test="${not empty fn:trim(action) && action eq 'popUpProfile'}">
            setTimeout(function() {
                openProfilePopup()
            }, 500); // Top 영역이 로드될 때까지 약간의 지연
        </c:if>
    });
</script>