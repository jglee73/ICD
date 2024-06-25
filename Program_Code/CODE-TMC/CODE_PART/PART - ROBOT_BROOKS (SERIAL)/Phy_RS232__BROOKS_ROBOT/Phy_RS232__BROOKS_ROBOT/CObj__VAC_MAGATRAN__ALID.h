#pragma once

#include "CCommon_SYSTEM.h"


// ...
#define  ACT__RETRY										"RETRY"
#define  ACT__IGNORE									"IGNORE"
#define  ACT__ABORT										"ABORT"
#define  ACT__YES										"YES"
#define  ACT__NO										"NO"


// ...
#define  ALID__OFFLINE_ALARM_MON						11
#define  ALID__OFFLINE_ALARM_ACT						12

//
#define  ALID__TM_CHM__NOT_ATM_PRESSURE					21
#define  ALID__LBA__NOT_ATM_PRESSURE					22
#define  ALID__LBB__NOT_ATM_PRESSURE					23
#define  ALID__PMx__NOT_ATM_PRESSURE					25

//
#define  ALID__TM_CHM__NOT_VAC_PRESSURE					31
#define  ALID__LBA__NOT_VAC_PRESSURE					32
#define  ALID__LBB__NOT_VAC_PRESSURE					33

#define  ALID__PMx__NOT_VAC_PRESSURE					35

//
#define  ALID__LBA__NOT_VALVE_OPEN									41
#define  ALID__LBB__NOT_VALVE_OPEN									42

#define  ALID__PMx__NOT_VALVE_OPEN									45
#define  ALID__AFTER_CALCULATION_R_OFFSET__RANGE_ERROR				46
#define  ALID__AFTER_CALCULATION_T_OFFSET__RANGE_ERROR				47

#define  ALID__AFTER_CALCULATION_R_OFFSET__SOFT_RANGE_ERROR			48
#define  ALID__AFTER_CALCULATION_T_OFFSET__SOFT_RANGE_ERROR			49
#define  ALID__AFTER_CALCULATION_R_OFFSET__HARD_RANGE_ERROR			50
#define  ALID__AFTER_CALCULATION_T_OFFSET__HARD_RANGE_ERROR			51


// Controller Error ID
#define  ALID__START         							101
#define  ALID__VACRB1_ERR_00001							102
#define  ALID__VACRB1_ERR_00210							103
#define  ALID__VACRB1_ERR_00220							104
#define  ALID__VACRB1_ERR_00221							105	
#define  ALID__VACRB1_ERR_00233							106
#define  ALID__VACRB1_ERR_00234							107
#define  ALID__VACRB1_ERR_00301							108
#define  ALID__VACRB1_ERR_00305							109
#define  ALID__VACRB1_ERR_00306							110

#define  ALID__VACRB1_ERR_00309							111
#define  ALID__VACRB1_ERR_00350							112
#define  ALID__VACRB1_ERR_00351							113
#define  ALID__VACRB1_ERR_00352							114
#define  ALID__VACRB1_ERR_00353							115
#define  ALID__VACRB1_ERR_00390							116
#define  ALID__VACRB1_ERR_00402							117
#define  ALID__VACRB1_ERR_00405							118
#define  ALID__VACRB1_ERR_00406							119
#define  ALID__VACRB1_ERR_00407							120

#define  ALID__VACRB1_ERR_00408							121
#define  ALID__VACRB1_ERR_00409							122
#define  ALID__VACRB1_ERR_00414							123
#define  ALID__VACRB1_ERR_00415							124
#define  ALID__VACRB1_ERR_00416							125
#define  ALID__VACRB1_ERR_00417							126
#define  ALID__VACRB1_ERR_00418							127
#define  ALID__VACRB1_ERR_00508							128
#define  ALID__VACRB1_ERR_00509							129
#define  ALID__VACRB1_ERR_00527							130

#define  ALID__VACRB1_ERR_00528							131
#define  ALID__VACRB1_ERR_00550							132
#define  ALID__VACRB1_ERR_00551							133
#define  ALID__VACRB1_ERR_00552							134
#define  ALID__VACRB1_ERR_00554							135
#define  ALID__VACRB1_ERR_00555							136
#define  ALID__VACRB1_ERR_00557							137
#define  ALID__VACRB1_ERR_00558							138
#define  ALID__VACRB1_ERR_00602							139
#define  ALID__VACRB1_ERR_00603							140

#define  ALID__VACRB1_ERR_00604							141
#define  ALID__VACRB1_ERR_00605							142
#define  ALID__VACRB1_ERR_00606							143
#define  ALID__VACRB1_ERR_00607							144
#define  ALID__VACRB1_ERR_00608							145
#define  ALID__VACRB1_ERR_00610							146
#define  ALID__VACRB1_ERR_00611							147
#define  ALID__VACRB1_ERR_00612							148
#define  ALID__VACRB1_ERR_00613							149
#define  ALID__VACRB1_ERR_00652							150

#define  ALID__VACRB1_ERR_00653							151
#define  ALID__VACRB1_ERR_00654							152
#define  ALID__VACRB1_ERR_00655							153
#define  ALID__VACRB1_ERR_00656							154
#define  ALID__VACRB1_ERR_00700							155
#define  ALID__VACRB1_ERR_00701							156
#define  ALID__VACRB1_ERR_00705							157
#define  ALID__VACRB1_ERR_00706							158
#define  ALID__VACRB1_ERR_00710							159
#define  ALID__VACRB1_ERR_00711							160

#define  ALID__VACRB1_ERR_00715							161
#define  ALID__VACRB1_ERR_00721							162
#define  ALID__VACRB1_ERR_00722							163
#define  ALID__VACRB1_ERR_00730							164
#define  ALID__VACRB1_ERR_00731							165
#define  ALID__VACRB1_ERR_00732							166
#define  ALID__VACRB1_ERR_00733							167
#define  ALID__VACRB1_ERR_00734							168
#define  ALID__VACRB1_ERR_00735							169
#define  ALID__VACRB1_ERR_00736							170

#define  ALID__VACRB1_ERR_00738							171
#define  ALID__VACRB1_ERR_00739							172
#define  ALID__VACRB1_ERR_00740							173
#define  ALID__VACRB1_ERR_00741							174
#define  ALID__VACRB1_ERR_00742							175
#define  ALID__VACRB1_ERR_00743							176
#define  ALID__VACRB1_ERR_00744							177
#define  ALID__VACRB1_ERR_00745							178
#define  ALID__VACRB1_ERR_00749							179
#define  ALID__VACRB1_ERR_00750							180

#define  ALID__VACRB1_ERR_00751							181
#define  ALID__VACRB1_ERR_00800							182
#define  ALID__VACRB1_ERR_00801							183
#define  ALID__VACRB1_ERR_00802							184
#define  ALID__VACRB1_ERR_00803							185
#define  ALID__VACRB1_ERR_00804							186
#define  ALID__VACRB1_ERR_00805							187
#define  ALID__VACRB1_ERR_00810							188
#define  ALID__VACRB1_ERR_00811							189
#define  ALID__VACRB1_ERR_00812							190

#define  ALID__VACRB1_ERR_00813							191
#define  ALID__VACRB1_ERR_00814							192
#define  ALID__VACRB1_ERR_00815							193
#define  ALID__VACRB1_ERR_00816							194
#define  ALID__VACRB1_ERR_00817							195
#define  ALID__VACRB1_ERR_00818							196
#define  ALID__VACRB1_ERR_00819							197
#define  ALID__VACRB1_ERR_00820							198
#define  ALID__VACRB1_ERR_00821							199
#define  ALID__VACRB1_ERR_00822							200

#define  ALID__VACRB1_ERR_00823							201
#define  ALID__VACRB1_ERR_00824							202
#define  ALID__VACRB1_ERR_00825							203
#define  ALID__VACRB1_ERR_00826							204
#define  ALID__VACRB1_ERR_00827							205
#define  ALID__VACRB1_ERR_00850							206
#define  ALID__VACRB1_ERR_00851							207
#define  ALID__VACRB1_ERR_00852							208
#define  ALID__VACRB1_ERR_00853							209
#define  ALID__VACRB1_ERR_00854							210

#define  ALID__VACRB1_ERR_00855							211
#define  ALID__VACRB1_ERR_00857							212
#define  ALID__VACRB1_ERR_00860							213
#define  ALID__VACRB1_ERR_00861							214
#define  ALID__VACRB1_ERR_00862							215
#define  ALID__VACRB1_ERR_00950							216
#define  ALID__VACRB1_ERR_00951							217
#define  ALID__VACRB1_ERR_00952							218
#define  ALID__VACRB1_ERR_00953							219
#define  ALID__VACRB1_ERR_00954							220

#define  ALID__VACRB1_ERR_00955							221
#define  ALID__VACRB1_ERR_00956							222
#define  ALID__VACRB1_ERR_01001							223
#define  ALID__VACRB1_ERR_01002							224
#define  ALID__VACRB1_ERR_01003							225
#define  ALID__VACRB1_ERR_01004							226
#define  ALID__VACRB1_ERR_01005							227
#define  ALID__VACRB1_ERR_01006							228
#define  ALID__VACRB1_ERR_01007							229
#define  ALID__VACRB1_ERR_01008							230

#define  ALID__VACRB1_ERR_01009							231
#define  ALID__VACRB1_ERR_01010							232
#define  ALID__VACRB1_ERR_01011							233
#define  ALID__VACRB1_ERR_01012							234
#define  ALID__VACRB1_ERR_01013							235
#define  ALID__VACRB1_ERR_01014							236
#define  ALID__VACRB1_ERR_01015							237
#define  ALID__VACRB1_ERR_01100							238
#define  ALID__VACRB1_ERR_01101							239
#define  ALID__VACRB1_ERR_01102							240

#define  ALID__VACRB1_ERR_01103							241
#define  ALID__VACRB1_ERR_01104							242
#define  ALID__VACRB1_ERR_01105							243
#define  ALID__VACRB1_ERR_01106							244
#define  ALID__VACRB1_ERR_01108							245
#define  ALID__VACRB1_ERR_01109							246
#define  ALID__VACRB1_ERR_01110							247
#define  ALID__VACRB1_ERR_01113							248
#define  ALID__VACRB1_ERR_01114							249
#define  ALID__VACRB1_ERR_01115							250

#define  ALID__VACRB1_ERR_01118							251
#define  ALID__VACRB1_ERR_01119							252
#define  ALID__VACRB1_ERR_01300							253
#define  ALID__VACRB1_ERR_01302							254
#define  ALID__VACRB1_ERR_01307							255
#define  ALID__VACRB1_ERR_01308							256
#define  ALID__VACRB1_ERR_01309							257
#define  ALID__VACRB1_ERR_01310							258
#define  ALID__VACRB1_ERR_01311							259
#define  ALID__VACRB1_ERR_01312							260

#define  ALID__VACRB1_ERR_01313							261
#define  ALID__VACRB1_ERR_01314							262
#define  ALID__VACRB1_ERR_01600							263
#define  ALID__VACRB1_ERR_01601							264
#define  ALID__VACRB1_ERR_01602							265
#define  ALID__VACRB1_ERR_01603							266
#define  ALID__VACRB1_ERR_01604							267
#define  ALID__VACRB1_ERR_01605							268
#define  ALID__VACRB1_ERR_01606							269
#define  ALID__VACRB1_ERR_01607							270

#define  ALID__VACRB1_ERR_01800							271
#define  ALID__VACRB1_ERR_01801							272
#define  ALID__VACRB1_ERR_01802							273
#define  ALID__VACRB1_ERR_01803							274
#define  ALID__VACRB1_ERR_01804							275
#define  ALID__VACRB1_ERR_01805							276
#define  ALID__VACRB1_ERR_01900							277
#define  ALID__VACRB1_ERR_01901							278
#define  ALID__VACRB1_ERR_01902							279
#define  ALID__VACRB1_ERR_01903							280

#define  ALID__VACRB1_ERR_01904							281
#define  ALID__VACRB1_ERR_01905							282
#define  ALID__VACRB1_ERR_01910							283
#define  ALID__VACRB1_ERR_01911							284
#define  ALID__VACRB1_ERR_01912							285
#define  ALID__VACRB1_ERR_01920							286
#define  ALID__VACRB1_ERR_02000							287
#define  ALID__VACRB1_ERR_02001							288
#define  ALID__VACRB1_ERR_02002							289
#define  ALID__VACRB1_ERR_02003							290

#define  ALID__VACRB1_ERR_02004							291
#define  ALID__VACRB1_ERR_02005							292
#define  ALID__VACRB1_ERR_02100							293
#define  ALID__VACRB1_ERR_02101							294
#define  ALID__VACRB1_ERR_02102							295
#define  ALID__VACRB1_ERR_02200							296
#define  ALID__VACRB1_ERR_02202							297
#define  ALID__VACRB1_ERR_02203							298
#define  ALID__VACRB1_ERR_03000							299
#define  ALID__VACRB1_ERR_03001							300

#define  ALID__VACRB1_ERR_03002							301
#define  ALID__VACRB1_ERR_03003							302
#define  ALID__VACRB1_ERR_03004							303
#define  ALID__VACRB1_ERR_03005							304
#define  ALID__VACRB1_ERR_03011							305
#define  ALID__VACRB1_ERR_04001							306
#define  ALID__VACRB1_ERR_04002							307
#define  ALID__VACRB1_ERR_04003							308
#define  ALID__VACRB1_ERR_04004							309
#define  ALID__VACRB1_ERR_04005							310

#define  ALID__VACRB1_ERR_04006							311
#define  ALID__VACRB1_ERR_04007							312
#define  ALID__VACRB1_ERR_04008							313
#define  ALID__VACRB1_ERR_04009							314
#define  ALID__VACRB1_ERR_04010							315
#define  ALID__VACRB1_ERR_04011							316
#define  ALID__VACRB1_ERR_04012							317
#define  ALID__VACRB1_ERR_10000							318
#define  ALID__VACRB1_ERR_10001							319
#define  ALID__VACRB1_ERR_10002							320

#define  ALID__VACRB1_ERR_10003							321
#define  ALID__VACRB1_ERR_10004							322
#define  ALID__VACRB1_ERR_10005							323
#define  ALID__VACRB1_ERR_10006							324
#define  ALID__VACRB1_ERR_10007							325
#define  ALID__VACRB1_ERR_10008							326
#define  ALID__VACRB1_ERR_10009							327
#define  ALID__VACRB1_ERR_10010							328
#define  ALID__VACRB1_ERR_10011							329
#define  ALID__VACRB1_ERR_10012							330

#define  ALID__VACRB1_ERR_10013							331
#define  ALID__VACRB1_ERR_10014							332
#define  ALID__VACRB1_ERR_10015							333
#define  ALID__VACRB1_ERR_10016							334
#define  ALID__VACRB1_ERR_10017							335
#define  ALID__VACRB1_ERR_10018							336
#define  ALID__VACRB1_ERR_10019							337
#define  ALID__VACRB1_ERR_10020							338
#define  ALID__VACRB1_ERR_10021							339
#define  ALID__VACRB1_ERR_10022							340

#define  ALID__VACRB1_ERR_10023							341
#define  ALID__VACRB1_ERR_10024							342
#define  ALID__VACRB1_ERR_10025							343
#define  ALID__VACRB1_ERR_10026							344
#define  ALID__VACRB1_ERR_10028							345
#define  ALID__VACRB1_ERR_10029							346
#define  ALID__VACRB1_ERR_10030							347
#define  ALID__VACRB1_ERR_10031							348
#define  ALID__VACRB1_ERR_10032							349
#define  ALID__VACRB1_ERR_10034							350

#define  ALID__VACRB1_ERR_10035							351
#define  ALID__VACRB1_ERR_10036							352

#define  ALID__VACRB1_ERR_Unknown						353

#define  ALID__VACRB1_MATERIAL_DIFFERENT_INFO			354

#define  ALID__END         							    355
