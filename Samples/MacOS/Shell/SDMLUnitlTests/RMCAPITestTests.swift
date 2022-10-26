//
//  RMCAPITestTests.swift
//  RMCAPITestTests
//
//  Created by jfang on 4/16/18.
//  Copyright © 2018 Nextlabs. All rights reserved.
//

import XCTest
import SDML

class RMCAPITest {
    struct Constant{
        static let timeout: TimeInterval = 60
        static let user404Response = "{\"statusCode\":400,\"message\":\"The account does not exist.\",\"serverTime\":1515116533364}"
        //skydrm.com server response
        static let router = "https://rmtest.nextlabs.solutions"
        static let tenant = "skydrm.com"
        static let localRouter = "https://rms-centos7303.qapf1.qalab01.nextlabs.com:8443"
        static let localTenant = "74ff9f18-c502-493a-a1e2-18740e3dc356"
        static let johnResponse = "{\"statusCode\":200,\"message\":\"Authorized\",\"serverTime\":1530588630576,\"extra\":{\"userId\":1,\"ticket\":\"A608EFF29080372CD2FAF25164847D0E\",\"tenantId\":\"623a79c2-6f59-4273-b966-17b9c84c78fd\",\"ttl\":1533180629240,\"name\":\"John Tyler\",\"email\":\"john.tyler@qapf1.qalab01.nextlabs.com\",\"memberships\":[{\"id\":\"m1@74ff9f18-c502-493a-a1e2-18740e3dc356\",\"type\":0,\"tenantId\":\"623a79c2-6f59-4273-b966-17b9c84c78fd\",\"projectId\":1},{\"id\":\"m3@t-86f868a5e0704fc28f3700d08f2bdc8c\",\"type\":0,\"tenantId\":\"c275abf7-32cb-4f7a-b5f3-102b26b2a745\",\"projectId\":2},{\"id\":\"m5@t-8b73fdf7792f4d238623ef4057237511\",\"type\":0,\"tenantId\":\"491bb48c-2b99-4a96-b04a-6dc4e2f5c8a5\",\"projectId\":3},{\"id\":\"m16@t-6261707a816a403d97f36c397992666d\",\"type\":0,\"tenantId\":\"bf600a60-5bf6-4866-af78-f291aa4247e5\",\"projectId\":7},{\"id\":\"m10@t-5dd99ed8d4a94967947c0192f9abce2b\",\"type\":0,\"tenantId\":\"71083283-5de1-4c91-aaec-f259ea3894d4\",\"projectId\":5},{\"id\":\"m13@t-c204283819cb4b6e8ef16d84e9075d6b\",\"type\":0,\"tenantId\":\"2cace1e1-e1b4-41a3-97a6-17a05c5d421a\",\"projectId\":6}],\"defaultTenant\":\"74ff9f18-c502-493a-a1e2-18740e3dc356\",\"defaultTenantUrl\":\"https://rms-centos7303.qapf1.qalab01.nextlabs.com:8444/rms\",\"attributes\":{\"aduser\":[\"John.tyler\"],\"addomain\":[\"qapf1.qalab01.nextlabs.com\"],\"displayName\":[\"John Tyler\"],\"objectSid\":[\"S-1-5-21-2018228179-1005617703-974104760-187941\"],\"memberOf\":[\"CN=ReportingGroup {16a91a21-092d-4c18-9763-e3ab4c2b51eb},DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=ReportingGroup {b85923ea-a73f-4404-a4cc-f1897ac091ef},DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=ReportingGroup {0c59db54-317b-41cf-a6d4-8535c4c8980e},OU=ADFS,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=ReportingGroup {25891af4-174c-4988-b9e0-9cf2fa1803ae},DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=ReportingGroup {859274f4-e5f3-4b6d-8e69-6ac4db0cb6c0},DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=ReportingGroup {8f16d24c-d1d1-4db0-9b2c-ea3a70832dc5},DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=ReportingGroup {106c3ad6-c5dc-426e-ab49-b12cfcd62411},DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=ReportingGroup {75178155-c5d4-4974-af3d-b1f92b5ab2c8},DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=ReportingGroup {70d3f3fc-18da-4ad7-a6d3-69f8f72c4454},DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=RMSAuto,OU=Groups,OU=Presidents,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=cc_group,OU=Groups,OU=Presidents,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=licenses test 4,OU=OEtestGroup,OU=Groups,OU=Presidents,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=licenses test 3,OU=OEtestGroup,OU=Groups,OU=Presidents,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=licenses test 1,OU=OEtestGroup,OU=Groups,OU=Presidents,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=SC,OU=Groups,OU=Presidents,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=TestGroup,CN=Users,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=rms-test,OU=Groups,OU=Presidents,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=AD,OU=Groups,OU=Presidents,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=HR,OU=Groups,OU=Presidents,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=G1,OU=groups,OU=small group,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=Presidential Soccer Team,OU=Groups,OU=Presidents,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=Strategic Marketing,OU=Groups,OU=Presidents,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=Business Marketing,OU=Groups,OU=Presidents,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=Domain Admins,CN=Users,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\",\"CN=tsgroup42,OU=Groups,OU=Presidents,DC=qapf1,DC=qalab01,DC=nextlabs,DC=com\"],\"email\":[\"john.tyler@qapf1.qalab01.nextlabs.com\"],\"adpass\":[\"1701D21E02C9CCF0D14EFA74C313D0A3\"]}}}"
        static let johncookie = "[\n  {\n    \"Value\" : \"John.tyler%40qapf1.qalab01.nextlabs.com\",\n    \"Name\" : \"userEmail\",\n    \"Domain\" : \"rms-centos7303.qapf1.qalab01.nextlabs.com\",\n    \"Path\" : \"\\/\"\n  }\n]"
        static let rmLoginURL = "https://rmtest.nextlabs.solutions/rms/Login.jsp?tenant=skydrm.com"
        static let serverResponse = "{\"statusCode\":200,\"message\":\"Authorized\",\"serverTime\":1524551551712,\"extra\":{\"userId\":385,\"ticket\":\"AF7EC36C93132D416733717FCF78941C\",\"tenantId\":\"8a0ebf44-dfdd-4261-85a3-c6dbc665358d\",\"ttl\":1527143551707,\"name\":\"JULIE BAOBAObbb\",\"email\":\"1059081636@qq.com\",\"preferences\":{\"homeTour\":true},\"idpType\":0,\"memberships\":[{\"id\":\"m600@skydrm.com\",\"type\":0,\"tenantId\":\"8a0ebf44-dfdd-4261-85a3-c6dbc665358d\",\"projectId\":1},{\"id\":\"m625@t-0c365880c230456a821e8f7df86f66da\",\"type\":0,\"tenantId\":\"1ba6a5d9-e320-4b4b-8710-4b9a5ea9e2db\",\"projectId\":110},{\"id\":\"m609@t-6188eb16b5344bec8d3fcc09e84fc17f\",\"type\":0,\"tenantId\":\"1832bdcb-1cb6-40f4-ad16-34286947bb35\",\"projectId\":98},{\"id\":\"m845@t-bab7d64590a0410884a5af499dbd7079\",\"type\":0,\"tenantId\":\"5485e37d-606e-400e-917d-f59f9d4880c0\",\"projectId\":226},{\"id\":\"m963@t-c61928dc62db46fa8b0e026e699833e9\",\"type\":0,\"tenantId\":\"17ebe688-d391-4036-a8df-536b0792d54f\",\"projectId\":104},{\"id\":\"m964@t-fb1c35c20b2748e69427605f34199478\",\"type\":0,\"tenantId\":\"a5049746-bf4d-4787-a726-8f57ea480920\",\"projectId\":126},{\"id\":\"m965@t-25a2593649894bd6b3fb4311aef6819a\",\"type\":0,\"tenantId\":\"99447317-432a-4207-9f51-fabefb391df1\",\"projectId\":125},{\"id\":\"m1034@t-b5dc8d8c07084320ae2c00723b898b45\",\"type\":0,\"tenantId\":\"fd0dfd21-53bd-47ee-98cb-14a281a04ae5\",\"projectId\":303},{\"id\":\"m1033@t-79a3c6310fcf4358b05dc1a585b52220\",\"type\":0,\"tenantId\":\"1fdca8f8-d520-408a-8cae-c593b27ce6cb\",\"projectId\":302}],\"defaultTenant\":\"skydrm.com\",\"defaultTenantUrl\":\"https://skydrm.com/rms\"}}"
        //skydrm.com cookie
        static let cookieResponse = "[\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"clientId\",\n    \"Comment\" : \"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"9A1C52684AA52CC46913E716CF8A01B9\",\n    \"Domain\" : \".skydrm.com\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"idp\",\n    \"Comment\" : \"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"0\",\n    \"Domain\" : \".skydrm.com\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"lt\",\n    \"Comment\" : \"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"skydrm.com\",\n    \"Domain\" : \".skydrm.com\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"platformId\",\n    \"Comment\" : \"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"300\",\n    \"Domain\" : \".skydrm.com\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"tenantId\",\n    \"Comment\" : \"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"8a0ebf44-dfdd-4261-85a3-c6dbc665358d\",\n    \"Domain\" : \".skydrm.com\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"ticket\",\n    \"Comment\" : \"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"AF7EC36C93132D416733717FCF78941C\",\n    \"Domain\" : \".skydrm.com\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"userId\",\n    \"Comment\" : \"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"385\",\n    \"Domain\" : \".skydrm.com\"\n  }\n]"
        //rmtest.nextlabs.solutions server response
        static let rmtest = "{\"statusCode\":200,\"message\":\"Authorized\",\"serverTime\":1531276875825,\"extra\":{\"userId\":36,\"ticket\":\"0B066F547B8981C86E65443520D2FA49\",\"tenantId\":\"21b06c79-baab-419d-8197-bad3ce3f4476\",\"lt\":\"skydrm.com\",\"ltId\":\"21b06c79-baab-419d-8197-bad3ce3f4476\",\"ttl\":1532571293246,\"name\":\"Julie wawa\",\"email\":\"julie.fang@cn.nextlabs.com\",\"preferences\":{\"homeTour\":true},\"idpType\":0,\"memberships\":[{\"id\":\"m66@skydrm.com\",\"type\":0,\"tenantId\":\"21b06c79-baab-419d-8197-bad3ce3f4476\",\"projectId\":1},{\"id\":\"m314@t-5f037f3122394942b954ff748730149b\",\"type\":0,\"tenantId\":\"89735d4e-23b2-4f5c-8664-cf0b2445faaa\",\"projectId\":150},{\"id\":\"m563@t-cf1aad527ccd485d98512366a11b1055\",\"type\":0,\"tenantId\":\"61797807-b3dd-413c-9e9d-04317833cd5f\",\"projectId\":107},{\"id\":\"m564@t-2f8bea9630854e7383fb7a42aa6232bd\",\"type\":0,\"tenantId\":\"a65076c1-53f1-4051-be51-78b115e34279\",\"projectId\":132},{\"id\":\"m3238@t-32915579fd0e4d62ac83a56fbf4cb37e\",\"type\":0,\"tenantId\":\"6a70ba80-d820-4345-9127-b10212cc38f3\",\"projectId\":2057},{\"id\":\"m663@t-c7547dadc4e94d7abc928eb58ba8b63c\",\"type\":0,\"tenantId\":\"4bf3e6ec-e30c-47cd-bf85-3df80c44d08a\",\"projectId\":312},{\"id\":\"m666@t-edaa916da10a419a9e92320527583e4e\",\"type\":0,\"tenantId\":\"c49de0ea-b20d-49b4-a17b-ab5efc42a8ca\",\"projectId\":356},{\"id\":\"m1260@t-8977e4317f8d48b093f2c1476b0b297a\",\"type\":0,\"tenantId\":\"84b629aa-bd74-4e62-9719-678d20e1d049\",\"projectId\":662},{\"id\":\"m1483@t-0c69200cee4d4397863974ff085f294d\",\"type\":0,\"tenantId\":\"655d6d17-716c-4ad8-b398-8b67fd702825\",\"projectId\":39},{\"id\":\"m1464@t-675e162b9c1f43e89f17d3a5c3b16414\",\"type\":0,\"tenantId\":\"b4d5a021-fcb2-4924-8a23-b6cdf175fad5\",\"projectId\":112},{\"id\":\"m1659@t-c13e734ca27d47e9abf826b02dfb6ef0\",\"type\":0,\"tenantId\":\"c24e9b59-db11-41ab-89fe-cb612b5648b5\",\"projectId\":963},{\"id\":\"m1661@t-d59abd06ae2b4f6ea6f4135b58cbda09\",\"type\":0,\"tenantId\":\"3dfe683a-059c-4582-be89-f47050710899\",\"projectId\":964},{\"id\":\"m1663@t-a95cb967f2e84cc79c87673014828b94\",\"type\":0,\"tenantId\":\"7b7147da-20fc-46c9-9b39-acb3fcee6097\",\"projectId\":966},{\"id\":\"m1664@t-19fdad827e60433ca61230be0320b0b8\",\"type\":0,\"tenantId\":\"744c87a2-5285-4343-8f0b-cb1c68b321ac\",\"projectId\":967},{\"id\":\"m1784@t-c0b8fa8074464fffbee3b44ebf9e5070\",\"type\":0,\"tenantId\":\"4b2eddc9-3374-4c93-a15f-a2317b236d15\",\"projectId\":1051},{\"id\":\"m1778@t-8bcc370adb694872af3184945e396b66\",\"type\":0,\"tenantId\":\"9865b079-7467-4c7c-9c5d-fd61feb77e03\",\"projectId\":1047},{\"id\":\"m1782@t-1fd58629ef304087861af2a774979267\",\"type\":0,\"tenantId\":\"3a8e3bdd-44c5-4643-97b4-c3143e9979b6\",\"projectId\":1049},{\"id\":\"m1783@t-f7170f4ae1f540d89a941a68cc9e2825\",\"type\":0,\"tenantId\":\"11772b0d-3fc5-48d2-972d-55fd538ec289\",\"projectId\":1050},{\"id\":\"m1785@t-b6695dbf47564a6a9881dd0628f3f50b\",\"type\":0,\"tenantId\":\"bc18d528-6038-417e-b70d-e3a660717f87\",\"projectId\":1052},{\"id\":\"m1779@t-c8a04febe6d94375bc2ff41b9f232b80\",\"type\":0,\"tenantId\":\"f4968206-240b-4809-a95d-c275ee67ffe9\",\"projectId\":1048},{\"id\":\"m1802@t-608a75f547dc4a00ad78a54d9fd6e6e4\",\"type\":0,\"tenantId\":\"154fef57-bd06-4804-a4a6-62359779f532\",\"projectId\":1065},{\"id\":\"m1977@t-14c58f2e40c1499fa5ee965707941594\",\"type\":0,\"tenantId\":\"07b451df-fb2e-4849-9e79-a5b447067cc7\",\"projectId\":1208},{\"id\":\"m1978@t-e412e3989f4742afb80ccc63492d5387\",\"type\":0,\"tenantId\":\"05b4755d-980a-4745-a507-b927dc3b5ea6\",\"projectId\":1209},{\"id\":\"m2045@t-ed14e64aabc5490d9464c50c047cfaf2\",\"type\":0,\"tenantId\":\"0d5507ce-c0ae-46bc-9024-a70662e4398a\",\"projectId\":1239},{\"id\":\"m2046@t-f720dcc855a64a0a9d76cb1cf12248bf\",\"type\":0,\"tenantId\":\"3aeb653d-94bc-47a9-ad19-e841443a187c\",\"projectId\":1240},{\"id\":\"m2047@t-e14a2c77572c40b3b9edac6fb1792857\",\"type\":0,\"tenantId\":\"ee2b5312-a3e2-44b0-9c00-15cd4fc2dcc4\",\"projectId\":1241},{\"id\":\"m2048@t-0f75bc43c4504184b0f62cd35fe02bcf\",\"type\":0,\"tenantId\":\"0092d9fb-6a7f-4265-8003-de8036d8d07b\",\"projectId\":1242},{\"id\":\"m235@t-c9e20096a2a84a419cbb28feb533e7bb\",\"type\":0,\"tenantId\":\"d31dc117-bec4-4dda-afee-49b9f194b82c\",\"projectId\":100},{\"id\":\"m2142@t-53b23eeb857c4348bc1bf0e56ce6d100\",\"type\":0,\"tenantId\":\"57e5f05f-647b-41eb-946d-a81846673783\",\"projectId\":1312},{\"id\":\"m3204@t-e27550c9336d4951971da14bd0881792\",\"type\":0,\"tenantId\":\"b8434c6d-c20b-4829-9bfb-cee65496c20a\",\"projectId\":2025},{\"id\":\"m3205@t-4159f5b196744910bcc46c034c2114a8\",\"type\":0,\"tenantId\":\"530a5410-1767-4121-bcf0-bb5a11442d9d\",\"projectId\":2026},{\"id\":\"m2542@t-6c1f811a61ea4e4daded26ca4d2fab29\",\"type\":0,\"tenantId\":\"579fc0f7-c749-417c-a61f-d86ed06e5533\",\"projectId\":1597},{\"id\":\"m2584@t-7ee9074fb2884617af5fa5fbfbe856de\",\"type\":0,\"tenantId\":\"6d89e300-d19f-494a-95fb-25548ba51a3e\",\"projectId\":1618},{\"id\":\"m2567@t-0cc7fbaeafc54d48b1059991554d4221\",\"type\":0,\"tenantId\":\"906e268b-f55b-4428-bbb5-ccc1ce10fa6b\",\"projectId\":1610},{\"id\":\"m2585@t-bf2f87cb10b54675846fae502938c66c\",\"type\":0,\"tenantId\":\"2b382cc3-1244-4342-94d5-4de38388eb08\",\"projectId\":1619},{\"id\":\"m2560@t-bdd16f69457b43d98c90a87155ab9ad6\",\"type\":0,\"tenantId\":\"c6bb7294-35af-4d6e-ab56-8586f91c250b\",\"projectId\":1604},{\"id\":\"m2596@t-fda168334b7e41c387f588d00121cc4f\",\"type\":0,\"tenantId\":\"5d51b068-0cd4-4ab6-a4bd-9316649a63ae\",\"projectId\":1622},{\"id\":\"m2615@t-1483fb9898fc4eb2afb82f37dd3803aa\",\"type\":0,\"tenantId\":\"1d02b96e-924b-4f61-a768-bcc6714a8f84\",\"projectId\":1633},{\"id\":\"m2608@t-d0cfce57c348472faa40def3602806a2\",\"type\":0,\"tenantId\":\"37e70fd4-944a-4a3b-9c13-a8825759f7dd\",\"projectId\":1629},{\"id\":\"m2603@t-bfb01af218514c809e36caeba0fe1a9c\",\"type\":0,\"tenantId\":\"292c81a3-6929-40f3-8200-b1c161d3ec6d\",\"projectId\":1627},{\"id\":\"m2612@t-91fce0c61f484168a0ccf34fc37d8ad7\",\"type\":0,\"tenantId\":\"fbcee195-320d-4630-84a9-ffa7b9007d0b\",\"projectId\":1631},{\"id\":\"m2605@t-8e0816353aba471485c5c1fb3d2e0a15\",\"type\":0,\"tenantId\":\"965ba76e-5e54-4e49-b7f7-8e142554fb70\",\"projectId\":1628},{\"id\":\"m2604@t-994db9cdb44443c79a92b705a8288c82\",\"type\":0,\"tenantId\":\"8799039f-d8dc-4bb5-bb56-657446dee3c5\",\"projectId\":1615},{\"id\":\"m2606@t-63874741b0c04850ba93148088cf862f\",\"type\":0,\"tenantId\":\"ef628f81-eadd-46dd-b04a-15f236546664\",\"projectId\":1070},{\"id\":\"m2618@t-a9063a03d083442e96e533b82bd98ca8\",\"type\":0,\"tenantId\":\"44ffc0c6-1896-4630-b938-dfc90dc7700e\",\"projectId\":1636},{\"id\":\"m2610@t-65bb3a223bcf4ee2bb8acb1c4cc8055f\",\"type\":0,\"tenantId\":\"409f5e79-50dd-45d6-a8b0-006544223695\",\"projectId\":1630},{\"id\":\"m2648@t-3a2cb9b8773e44cfadbcb94a98401df2\",\"type\":0,\"tenantId\":\"38761258-da3c-406c-9c74-2eeb908fd579\",\"projectId\":1664},{\"id\":\"m2652@t-f5c3e79edf7d4716a09e8399ef6c5b86\",\"type\":0,\"tenantId\":\"7ba25c0d-c3cb-4066-8f25-0d86227c0c38\",\"projectId\":1665},{\"id\":\"m2644@t-f5c5b0687ad9460395bc3bb89883b17e\",\"type\":0,\"tenantId\":\"f77acff1-3ede-43c8-87a0-2db1f5808ba0\",\"projectId\":1660},{\"id\":\"m2716@t-ddc68a7f4f264d9fae43ba83f212954e\",\"type\":0,\"tenantId\":\"8aa60128-4e37-4e44-96a9-5c57d5789ed5\",\"projectId\":1708},{\"id\":\"m2767@t-385e6aca683644f4ba4368c6aaa21e88\",\"type\":0,\"tenantId\":\"e3c057b9-64a6-4603-ad4e-745e5e5e4acc\",\"projectId\":1659}],\"defaultTenant\":\"skydrm.com\",\"defaultTenantUrl\":\"https://rmtest.nextlabs.solutions/rms\",\"attributes\":{\"displayName\":[\"Julie wawa\"],\"email\":[\"julie.fang@cn.nextlabs.com\"]}}}"
        //rmtest.nextlabs.solutions server cookie response
        static let rmtestcookie = "[\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"clientId\",\n    \"Comment\" : \"\\\"\\\"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"9A1C52684AA52CC46913E716CF8A01B9\",\n    \"Domain\" : \".rms-centos7303.qapf1.qalab01.nextlabs.com\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"idp\",\n    \"Comment\" : \"\\\"\\\"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"4\",\n    \"Domain\" : \".rms-centos7303.qapf1.qalab01.nextlabs.com\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"lt\",\n    \"Comment\" : \"\\\"\\\"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"74ff9f18-c502-493a-a1e2-18740e3dc356\",\n    \"Domain\" : \".rms-centos7303.qapf1.qalab01.nextlabs.com\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"ltId\",\n    \"Comment\" : \"\\\"\\\"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"623a79c2-6f59-4273-b966-17b9c84c78fd\",\n    \"Domain\" : \".rms-centos7303.qapf1.qalab01.nextlabs.com\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"platformId\",\n    \"Comment\" : \"\\\"\\\"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"300\",\n    \"Domain\" : \".rms-centos7303.qapf1.qalab01.nextlabs.com\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"tenantId\",\n    \"Comment\" : \"\\\"\\\"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"623a79c2-6f59-4273-b966-17b9c84c78fd\",\n    \"Domain\" : \".rms-centos7303.qapf1.qalab01.nextlabs.com\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"ticket\",\n    \"Comment\" : \"\\\"\\\"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"A608EFF29080372CD2FAF25164847D0E\",\n    \"Domain\" : \".rms-centos7303.qapf1.qalab01.nextlabs.com\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"userId\",\n    \"Comment\" : \"\\\"\\\"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"1\",\n    \"Domain\" : \".rms-centos7303.qapf1.qalab01.nextlabs.com\"\n  }\n]"
        
        static let rmTestJoanna = "{\"statusCode\":200,\"message\":\"Authorized\",\"serverTime\":1530175043070,\"extra\":{\"userId\":12,\"ticket\":\"407A4CF5E34CDEF9562BA83DC74A39B3\",\"tenantId\":\"21b06c79-baab-419d-8197-bad3ce3f4476\",\"lt\":\"skydrm.com\",\"ltId\":\"21b06c79-baab-419d-8197-bad3ce3f4476\",\"ttl\":1532767043068,\"name\":\"相 关流\",\"email\":\"joanna.chen@cn.nextlabs.com\",\"preferences\":{\"securityMode\":\"100\"},\"idpType\":0,\"memberships\":[{\"id\":\"m16@skydrm.com\",\"type\":0,\"tenantId\":\"21b06c79-baab-419d-8197-bad3ce3f4476\",\"projectId\":1},{\"id\":\"m264@t-675e162b9c1f43e89f17d3a5c3b16414\",\"type\":0,\"tenantId\":\"b4d5a021-fcb2-4924-8a23-b6cdf175fad5\",\"projectId\":112},{\"id\":\"m1420@t-1a013ad11a4c4429bb3fbf5d911b0e02\",\"type\":0,\"tenantId\":\"dd203d23-d8ab-43a5-8d02-53a740d75edb\",\"projectId\":785},{\"id\":\"m95@t-0c69200cee4d4397863974ff085f294d\",\"type\":0,\"tenantId\":\"655d6d17-716c-4ad8-b398-8b67fd702825\",\"projectId\":39},{\"id\":\"m1430@t-66c0229d2ce14ad583880f80fde73f20\",\"type\":0,\"tenantId\":\"a8ee36b2-afd4-4371-bf5b-c45b8c414f21\",\"projectId\":791},{\"id\":\"m1591@t-13e6b0e9c86447d1a0872919a303870c\",\"type\":0,\"tenantId\":\"87ca38f1-2b03-473c-b0f0-966e84c73f80\",\"projectId\":257},{\"id\":\"m1974@t-36f33702391a40bcb9535076cd3ea30d\",\"type\":0,\"tenantId\":\"135eb856-aebd-40f2-ac38-83408dd31ff1\",\"projectId\":1205},{\"id\":\"m1997@t-ec524272f58d410f972b9109c22ef662\",\"type\":0,\"tenantId\":\"9dd8ddd0-9f76-4fad-bad8-53e7c0cd2fec\",\"projectId\":1223},{\"id\":\"m2097@t-c0e1157bd04c429eb000597c5ce1a01c\",\"type\":0,\"tenantId\":\"1111534f-26f6-4f89-9f84-af11ef91d804\",\"projectId\":1277},{\"id\":\"m2129@t-68f8c1d6b83446a090c3a1e25ae1e6dd\",\"type\":0,\"tenantId\":\"ad053707-55d8-4c93-93e5-fbcae5b4ebce\",\"projectId\":1303},{\"id\":\"m2169@t-15f282eec90e4635a7e415f9a5d80975\",\"type\":0,\"tenantId\":\"6926699d-5ecd-4ec7-8ad4-5a986ce80447\",\"projectId\":1333},{\"id\":\"m2187@t-c81dfa9d05604fb1b09772eb836bfaa0\",\"type\":0,\"tenantId\":\"456538aa-0c8c-4162-9dcd-d7fbecb0824b\",\"projectId\":1348},{\"id\":\"m2182@t-2cb92974c2da46e1990137a9f3a8b4cd\",\"type\":0,\"tenantId\":\"a3d8b3fc-db9f-4346-a85a-1c141f1d4884\",\"projectId\":1343},{\"id\":\"m2221@t-231c828d0f3740b49fa4b8f30db6b23c\",\"type\":0,\"tenantId\":\"22bf4190-67de-421f-b826-d3916806bf26\",\"projectId\":1378},{\"id\":\"m5697@t-a4496e0e9e4646eeab3b2876ce500171\",\"type\":0,\"tenantId\":\"1e46cb96-12be-42b6-b76d-52813c280379\",\"projectId\":4425},{\"id\":\"m5698@t-893ed68d57af430283a429fcfb237bd7\",\"type\":0,\"tenantId\":\"2b292b54-1fe7-413f-91a0-4ad977b4de75\",\"projectId\":4426},{\"id\":\"m5699@t-475c2ba53aa84aa4a38951610e2975d9\",\"type\":0,\"tenantId\":\"351221e1-a56d-469f-a476-1d868e52fe10\",\"projectId\":4427},{\"id\":\"m5700@t-2c6b486ab8374da4a16d8551df29bc97\",\"type\":0,\"tenantId\":\"06ab670b-1ff5-40cd-a0cf-a76cea57ed40\",\"projectId\":4428},{\"id\":\"m5701@t-af07b8b20aa34730a542b7a95f61b6b9\",\"type\":0,\"tenantId\":\"ada9109d-8ce1-4b73-9d0c-cf12b6daa496\",\"projectId\":4429},{\"id\":\"m5702@t-ce3727cea698410890f47090009173a9\",\"type\":0,\"tenantId\":\"1deefd3e-8594-48aa-be85-a254295c932b\",\"projectId\":4430},{\"id\":\"m5705@t-51d43c678031411db9ffaee21e05edbd\",\"type\":0,\"tenantId\":\"e95716dd-f268-45a2-8853-46b44465f38b\",\"projectId\":4433},{\"id\":\"m5703@t-9d864a045ebb413e9bb974f9b8dc449b\",\"type\":0,\"tenantId\":\"bb7fe89b-7308-45d0-8eed-919086b36694\",\"projectId\":4431},{\"id\":\"m5704@t-ac077b3ed1704bd49b8b4111a5b90ae4\",\"type\":0,\"tenantId\":\"01e204cf-62a9-49dc-bc52-b595f8df59a4\",\"projectId\":4432},{\"id\":\"m5709@t-0d468856c36b40538c3217af229e89cd\",\"type\":0,\"tenantId\":\"8f228513-d249-496e-9956-760e5e5f2b87\",\"projectId\":4437},{\"id\":\"m5706@t-2edc70d2b368498f892b52fd7fc3a1c1\",\"type\":0,\"tenantId\":\"e432b595-4c31-420c-833b-bc8eb2e48f24\",\"projectId\":4434},{\"id\":\"m5707@t-0d000accc8324a6a877f7a9a01f5af94\",\"type\":0,\"tenantId\":\"fbcc3847-bdda-4685-a009-920320dad90b\",\"projectId\":4435},{\"id\":\"m5708@t-818f5b82092148eebda3ee1d8358021f\",\"type\":0,\"tenantId\":\"3e9636fa-b589-4a99-92b3-6a5d3aaa952c\",\"projectId\":4436}],\"defaultTenant\":\"skydrm.com\",\"defaultTenantUrl\":\"https://rmtest.nextlabs.solutions/rms\",\"attributes\":{\"displayName\":[\"相 关流\"],\"email\":[\"joanna.chen@cn.nextlabs.com\"]}}}"
        static let rmTestJoannaCookie = "[\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"clientId\",\n    \"Comment\" : \"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"9A1C52684AA52CC46913E716CF8A01B9\",\n    \"Domain\" : \".rmtest.nextlabs.solutions\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"idp\",\n    \"Comment\" : \"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"0\",\n    \"Domain\" : \".rmtest.nextlabs.solutions\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"lt\",\n    \"Comment\" : \"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"skydrm.com\",\n    \"Domain\" : \".rmtest.nextlabs.solutions\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"ltId\",\n    \"Comment\" : \"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"21b06c79-baab-419d-8197-bad3ce3f4476\",\n    \"Domain\" : \".rmtest.nextlabs.solutions\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"platformId\",\n    \"Comment\" : \"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"300\",\n    \"Domain\" : \".rmtest.nextlabs.solutions\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"tenantId\",\n    \"Comment\" : \"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"21b06c79-baab-419d-8197-bad3ce3f4476\",\n    \"Domain\" : \".rmtest.nextlabs.solutions\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"ticket\",\n    \"Comment\" : \"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"407A4CF5E34CDEF9562BA83DC74A39B3\",\n    \"Domain\" : \".rmtest.nextlabs.solutions\"\n  },\n  {\n    \"Secure\" : \"TRUE\",\n    \"Name\" : \"userId\",\n    \"Comment\" : \"\",\n    \"Path\" : \"\\/\",\n    \"Version\" : \"1\",\n    \"Value\" : \"12\",\n    \"Domain\" : \".rmtest.nextlabs.solutions\"\n  }\n]"
    }
    //get HTTP cookie from cookie string, then user can use the result to setLoginResult
    static func getHTTPCookiesFromString(from: String) -> [HTTPCookie]? {
        do {
            let data = from.data(using: .utf8)!
            let value = try JSONSerialization.jsonObject(with: data, options: .allowFragments)
            if let array = value as? [[String: String]] {
                var cookies = [HTTPCookie]()
                for dic in array {
                    var properties = [HTTPCookiePropertyKey: Any]()
                    transform(from: dic, to: &properties)
                    if let cookie = HTTPCookie(properties: properties) {
                        cookies.append(cookie)
                    }
                }
                return cookies
            }
        } catch {
            print(error)
        }
        
        return nil
    }
    
    static func transform(from: [String: String], to: inout [HTTPCookiePropertyKey: Any]) {
        for pair in from {
            to[HTTPCookiePropertyKey.init(pair.key)] = pair.value
        }
    }
    
    //how to get current time stamp, donot use this function yet
    static func testGetTimeInterval(){
        //        do {
        let now = Date()
        let timeInterval:TimeInterval = now.timeIntervalSince1970
        let timeStamp = Int(timeInterval)
        print("time for now: \(timeStamp)")
        //let timeStamp = 1524563705
        let timeInterval01:TimeInterval = TimeInterval(timeStamp)
        let date = NSDate(timeIntervalSince1970: timeInterval01)
        print("this date is \(date)")
        
        let dformatter = DateFormatter()
        dformatter.dateFormat = "yyyy-MM-dd-HH-mm"
        let timeStampNow = dformatter.string(from: date as Date)
        print("change date as: \(timeStampNow)")
        //        }
    }
    
    static func testReadTXTFile(casefile: String) -> [String]{
        var data = [String]()
        
        if let txtfile = Bundle.main.path(forResource: casefile, ofType: "txt") {
            do{
                let txtData = NSData.init(contentsOfFile: txtfile)
                let myString:String = (NSString.init(data: txtData as! Data, encoding: String.Encoding.utf8.rawValue))! as String
                let myStringasArray = myString.components(separatedBy: .newlines).dropLast()
                for em in myStringasArray{
                    data.append(em)
                }
            }catch{
                print(error)
            }
        }
        return(data)
    }
    
    static func getRights(fileRight: String) -> [SDMLFileRight]{
        var rights = [SDMLFileRight]()
        let rightNum = fileRight.components(separatedBy: ",")
        for ca in rightNum{
            if ca .isEmpty{
                //if no rights then: var rights = [] as! NSArray
                let noRight = [] as NSArray
                let rights = noRight
            }else{
                let num = Int(ca)!
                if num >= 4 {
                    print("----rights out of enum!---- \(ca)----")
                }
                let right = SDMLFileRight.init(rawValue: num)
                rights.append(right!)
            }
        }
        return(rights)
    }
    
    static func checkFileRights(nxlFileRights: [SDMLFileRight], fileRight: String) -> Bool{
        let result = Bool()
        let expectFileRights = getRights(fileRight: fileRight)
        if nxlFileRights == expectFileRights{
            print("--- nxl file rights is ture ---")
            return true
        }else{
            print("--- nxl file rights is not the expect one ---")
            return false
        }
        return(result)
    }
    
    static func getFileSize(file: String) -> Int{
        var size: Int = 0
        let fileManager = FileManager.default
        if fileManager.fileExists(atPath: file){
            do{
                let fileAttr = try fileManager.attributesOfItem(atPath: file)
                size += fileAttr[FileAttributeKey.size] as! Int
            }catch{
                print(error)
                print("----get file size failed----")
            }
        }
        return(size)
    }
    
    static func checkFileSize(nxlFile: String, originalFile: String) -> Bool{
        let result = Bool()
        let nxlFileSize = RMCAPITest.getFileSize(file: nxlFile)
        let originalFileSize = RMCAPITest.getFileSize(file: originalFile)
        if nxlFileSize >= originalFileSize{
            print("--- nxl file size is correct ---")
            return true
        }else{
            print("--- nxl file size is less then original file ---")
            return false
        }
        return result
    }
    
    static func getRecipients(emailAddress: String) -> [String]{
        var recipients = [String]()
        let emails = emailAddress.components(separatedBy: ",")
        for e in emails{
            recipients.append(e)
        }
        return(recipients)
    }
}

