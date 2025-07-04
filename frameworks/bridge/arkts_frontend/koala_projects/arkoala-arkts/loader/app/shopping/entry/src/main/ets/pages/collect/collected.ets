/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
import mediaquery from '@ohos.mediaquery'

import mediaQuery from '@ohos.mediaquery'
import router from '@ohos.router'
import { NullPage } from './nullPage'
import { SinglePage1 } from './singlePage1'
import { SinglePage2 } from './singlePage2'
import { SinglePage3 } from './singlePage3'

@Entry
@Component
struct CollectedPage {
  @State width1: number = 10
  @State ratio: number = 1
  @State opacity1: number = 1
  @State num: number = 1
  @State layoutId: number = 0
  @State collected: boolean = true
  // listener = mediaQuery.matchMediaSync('(orientation:landscape)')

  onPageShow() {
    const params = router.getParams() as Record<number, string>; // 获取传递过来的参数对象
    this.width1 = params['num']
    this.ratio = params['ratio']
  }

  aboutToAppear() {
    console.log('www data log ' + this.width)
    // this.listener.on('change', this.onPortrait)
  }

  async onPortrait(mediaQueryResult : mediaquery.MediaQueryResult) {
    let result = mediaQueryResult.matches
    if (result) {
      this.width1 = 45
    } else {
      this.width1 = 100
    }
  }


  build() {
    Column() {
      Column() {
        Flex({ justifyContent: FlexAlign.SpaceBetween, alignItems: ItemAlign.Center }) {
          Image('/resources/icon/arrow_left.png')
            .width(30 * this.ratio)
            .aspectRatio(1)
            .onClick(() => {
              router.back()
            })
          Text() {
            Span('我的收藏')
              .fontSize(18 * this.ratio)
              .fontWeight(FontWeight.Bolder)
            Span(` (${this.num})`)
              .fontSize(15 * this.ratio)
          }

          Row() {
            Image('/resources/icon/search.png')
              .width(25 * this.ratio)
              .aspectRatio(1)
              .objectFit(ImageFit.Contain)
              .margin({ right: 10 })
            Text('管理')
              .fontSize(18 * this.ratio)
          }
        }
        .padding({ top: 10, left: 10, right: 10 })

        Tabs() {
          TabContent() {
            Stack({ alignContent: Alignment.TopEnd }) {
              Column() {
                if (this.layoutId === 0) {
                  Column() {
                    SinglePage1({ ratio: this.ratio })
                  }
                } else if (this.layoutId === 1) {
                  Column() {
                    SinglePage2({ ratio: this.ratio })
                  }
                } else {
                  Column() {
                    SinglePage3({ ratio: this.ratio })
                  }
                }
              }

              if (this.collected) {
                Image('/resources/icon/layout.png')
                  .width(20 * (this.ratio + 0.2))
                  .aspectRatio(1)
                  .objectFit(ImageFit.Contain)
                  .margin(7)
                  .onClick(() => {
                    if (this.layoutId >= 2) {
                      this.layoutId = 0
                    } else {
                      this.layoutId++
                    }
                  })
              }
            }
            .width('100%')
            .height('100%')
          }
          .tabBar('宝贝')

          TabContent() {
            NullPage({ str: '暂未收藏图片', ratio: this.ratio })
          }
          .tabBar('图文')

          TabContent() {
            NullPage({ str: '暂未收藏视频', ratio: this.ratio })
          }
          .tabBar('视频')

          TabContent() {
            NullPage({ str: '暂未收藏话题', ratio: this.ratio })
          }
          .tabBar('话题')

          TabContent() {
            NullPage({ str: '暂未收藏清单', ratio: this.ratio })
          }
          .tabBar('清单')
        }
        .barMode(BarMode.Fixed)
        .barWidth(370 * this.ratio)
        .barHeight(50 * this.ratio)
      }
      .backgroundColor('#dfe5e3')
      .width('100%')
    }
    .width('100%')
    .backgroundColor('#F9F9F9')
  }

  pageTransition() {
    PageTransitionExit({ duration: 0, curve: Curve.Linear })
      .onExit((type: RouteType, progress: number) => {
        this.opacity1 = 1
      })
    PageTransitionEnter({ duration: 0, curve: Curve.Linear })
      .onEnter((type: RouteType, progress: number) => {
        this.opacity1 = 1
      })
  }
}